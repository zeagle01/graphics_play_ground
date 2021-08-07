


#include "discrete_collision_detection.h"
#include <algorithm>
#include <atomic>
#include <intrin.h>

namespace clumsy_engine
{

	static unsigned long long insert_00_after_1(unsigned long long bits)
	{
		bits = (bits | (bits << 32)) & 0xFFFF00000000FFFFu;
		bits = (bits | (bits << 16)) & 0x00FF0000FF0000FFu;
		bits = (bits | (bits << 8)) & 0xF00F00F00F00F00Fu;
		bits = (bits | (bits << 4)) & 0x30C30C30C30C30C3u;
		return (bits | (bits << 2)) & 0x9249249249249249u;
	}

	template<typename Type> 
	inline Type atomic_CAS(Type* Address, Type Exp, Type Val) 
	{
		reinterpret_cast<std::atomic<Type>*>(Address)->compare_exchange_weak(Exp, Val); 
		return Exp; 
	}
	
	int compute_common_leading_zeros(std::pair<unsigned long long, int> m0, const std::vector<std::pair<unsigned long long, int>>& morton_codes, int node_index, int leaf_num)
	{
		if (node_index >= 0 && node_index < leaf_num)
		{
			return __lzcnt64(m0.first ^ morton_codes[node_index].first);
		}
		else
		{
			return -1;
		}
	}

	vec2i determine_range(const std::vector<std::pair<unsigned long long, int>> & morton_codes, int node_index,int leaf_num)
	{
		auto morton_code = morton_codes[node_index];
		int num_of_common_leading_zeros_left = compute_common_leading_zeros(morton_code, morton_codes, node_index - 1, leaf_num);
		int num_of_common_leading_zeros_right = compute_common_leading_zeros(morton_code, morton_codes, node_index + 1, leaf_num);

		int dir = num_of_common_leading_zeros_left < num_of_common_leading_zeros_right ? -1 : 1;

		int min_num_of_common_leading_zeros = std::min(num_of_common_leading_zeros_left, num_of_common_leading_zeros_right);

		int lmax = 2;
		while (compute_common_leading_zeros(morton_code, morton_codes, node_index + lmax * dir, leaf_num) > min_num_of_common_leading_zeros)
		{
			lmax *= 2;
		}

		int l = 0;
		for (int t = lmax / 2; t >= 1; t /= 2)
		{
			if (compute_common_leading_zeros(morton_code, morton_codes, node_index + (l + t) * dir, leaf_num) > min_num_of_common_leading_zeros)
			{
				l = l + t;
			}
		}

		int split_index = node_index + l * dir;
		if (split_index < node_index)
		{
			return { split_index,node_index };
		}
		else
		{
			return { node_index ,split_index};
		}
	}

	int find_split(const std::vector<std::pair<unsigned long long, int>>& morton_codes, int left, int right)
	{
		auto left_morton_code = morton_codes[left].first;
		auto right_morton_code = morton_codes[right].first;
		if (left_morton_code == right_morton_code)
		{
			return (left + right) >> 1;
		}

		int num_of_common_zeros = __lzcnt64(left_morton_code ^ right_morton_code);
		int split = left;
		int step = right - left;
		do
		{
			step = (step + 1) >> 1;
			int new_split = split + step;
			if (new_split < right)
			{
				auto split_morton_code = morton_codes[new_split].first;
				int split_common_zeros = __lzcnt64(left_morton_code ^ split_morton_code);
				if (split_common_zeros > num_of_common_zeros)
				{
					split = new_split;
				}
			}

		} while (step >> 1);

		return split;
	}

	float shift(float pos, unsigned long long bits)
	{
		return std::clamp<float>(pos * bits, 0.0f, bits - 1.0f);
	}

	unsigned long long encode_component(float pos, unsigned long long bits)
	{
		pos = shift(pos, bits);
		return insert_00_after_1(static_cast<unsigned long long>(pos));
	}

	unsigned long long encode_positions(float x, float y, float z, unsigned long long bits)
	{
		unsigned long long xx = encode_component(x, bits);
		unsigned long long yy = encode_component(y, bits);
		unsigned long long zz = encode_component(z, bits);
		return (xx << 2) + (yy << 1) + zz;
	}

	void Discrete_Collision_Detection::construct_ee_bvh1()
	{

		auto edges = get_value<data::Edge_Indice>();
		auto positions = get_value<data::Position>();

		int edge_num = edges.size() / 2;

		//compute representive position
		m_edge_representative_positions.resize(edge_num);
		for (int i = 0; i < edge_num; i++)
		{
			int v0 = edges[i * 2 + 0];
			int v1 = edges[i * 2 + 1];
			m_edge_representative_positions[i] = 0.5f * (positions[v0] + positions[v1]);
		}

		//compute aabb
		AABB<3> aabb;
		for (int i = 0; i < edge_num; i++)
		{
			int v0 = edges[i * 2 + 0];
			int v1 = edges[i * 2 + 1];
			aabb += positions[v0];
			aabb += positions[v1];
		}

		//encode position into z-order
		auto aabb_side_length = aabb.m_upper - aabb.m_lower;

		//auto max_side_length = matrix_math::max(aabb_side_length);

		aabb_side_length += get_uniform<3, 1, float>(1e-6f);
		std::vector<std::pair<unsigned long long, int>> encoded_records;
		for (int i = 0; i < edge_num; i++)
		{
			auto reprent_pos = (m_edge_representative_positions[i] - aabb.m_lower) / aabb_side_length;
			unsigned long long encoded_position = encode_positions(reprent_pos(0),reprent_pos(1),reprent_pos(2),1ll << 21);
			encoded_records.push_back(std::make_pair(encoded_position, i));
		}

		//sort
		std::sort(std::begin(encoded_records), std::end(encoded_records), [](const auto& l, const auto& r) {return l.first < r.first; });

		std::vector<vec2i>					children(edge_num);
		std::vector<int>					parent(2 * edge_num - 1, -1);

		for (int i = 0; i < edge_num; i++)
		{
			vec2i range;
			if (i == 0)
			{
				parent[i] = -1;
				range = { 0,edge_num - 1 };
			}
			else
			{
				range = determine_range(encoded_records, i, edge_num);
			}

			int split = find_split(encoded_records, range(0), range(1));

			int child_left = (split == range(0)) ? (edge_num - 1 + split) : split;
			int child_right = (split + 1 == range(1)) ? (edge_num + split) : split + 1;

			parent[child_left] = i;
			parent[child_right] = i;
			if (child_right >= edge_num - 1)
			{
				std::swap(child_left, child_right);
			}

			children[i] = { child_left,child_right };

		}

	}

	void Discrete_Collision_Detection::construct_ee_bvh()
	{
		auto edges = get_value<data::Edge_Indice>();
		auto positions = get_value<data::Position>();

		int edge_num = edges.size() / 2;

		//compute representive position
		m_edge_representative_positions.resize(edge_num);
		for (int i = 0; i < edge_num; i++)
		{
			int v0 = edges[i * 2 + 0];
			int v1 = edges[i * 2 + 1];
			m_edge_representative_positions[i] = 0.5f * (positions[v0] + positions[v1]);
		}

		//compute aabb
		AABB<3> aabb;
		for (int i = 0; i < edge_num; i++)
		{
			int v0 = edges[i * 2 + 0];
			int v1 = edges[i * 2 + 1];
			aabb += positions[v0];
			aabb += positions[v1];
		}


		//encode position into z-order
		auto aabb_side_length = aabb.m_upper - aabb.m_lower;
		aabb_side_length += get_uniform<3, 1, float>(1e-6f);
		std::vector<std::pair<unsigned long long, int>> encoded_records;
		for (int i = 0; i < edge_num; i++)
		{
			auto reprent_pos = (m_edge_representative_positions[i] - aabb.m_lower) / aabb_side_length;

			constexpr unsigned long long m = 1ll << 21;
			reprent_pos(0) = std::clamp<float>(reprent_pos(0) * m, 0.0f, m - 1.0f);
			reprent_pos(1) = std::clamp<float>(reprent_pos(1) * m, 0.0f, m - 1.0f);
			reprent_pos(2) = std::clamp<float>(reprent_pos(2) * m, 0.0f, m - 1.0f);

			unsigned long long xx = insert_00_after_1(static_cast<unsigned long long>(reprent_pos(0)));
			unsigned long long yy = insert_00_after_1(static_cast<unsigned long long>(reprent_pos(1)));
			unsigned long long zz = insert_00_after_1(static_cast<unsigned long long>(reprent_pos(2)));

			unsigned long long encoded_position = (xx << 2) + (yy << 1) + zz;
			encoded_records.push_back(std::make_pair(encoded_position, i));
		}

		//sort
		std::sort(std::begin(encoded_records), std::end(encoded_records), [](const auto& l, const auto& r) {return l.first < r.first; });

		//common_prefixes
		std::vector<int> common_prefixes(edge_num);
		for (int i = 1; i < edge_num; i++)
		{
			common_prefixes[i] = __lzcnt64(encoded_records[i].first ^ encoded_records[i - 1].first);
		}


		//
		std::vector<int>					flags(edge_num - 1, -1);
		std::vector<vec2i>					children(edge_num);
		std::vector<int>					parent(edge_num, -1);
		std::vector<vec2i>					ranges(edge_num);

		int root = -1;
		for (int i = 0; i < edge_num; i++)
		{
			bool skip = true;
			auto  range = get_uniform<2, 1, int>(i);
			int current_node = edge_num - 1 + i;
			children[current_node] = get_uniform<2, 1, int>(encoded_records[i].second);
			do
			{
				int common_prefix_left = common_prefixes[range(0)];
				int common_prefix_right = common_prefixes[range(1) + 1];
				int is_left_node = common_prefix_left < common_prefix_right;
				int is_right_node = common_prefix_left >= common_prefix_right;
				int parent_index = is_right_node ? (range(0) - 1) : range(1);
				ranges[current_node] = range;
				parent[current_node] = parent_index;
				children[current_node](is_right_node) = current_node;
				int old_flag = atomic_CAS(&flags[parent_index], -1, current_node);
				if (old_flag != -1)
				{
					current_node = parent_index;
					auto cpl = children[parent_index](is_left_node);
					range(is_left_node) = ranges[cpl](is_left_node);
					if (range(1) - range(0) == edge_num - 1)
					{
						root = current_node;
						parent[root] = -1;
						skip = false;
					}
				}
			} while (skip == true);
		}
		m_root = root;

	}

	void Discrete_Collision_Detection::detection()
	{
		auto edges = get_value<data::Edge_Indice>();
		auto positions = get_value<data::Position>();
		int edge_num = edges.size() / 2;

		for (int i = 0; i < edge_num; i++)
		{
			auto edge = edges[i * 2 + 0];
			auto edge0 = edges[i * 2 + 1];
			//auto 
		}
	}

}
