


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

		}
	}

}
