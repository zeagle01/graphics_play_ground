

module;

#include <span>
#include <functional>
#include <algorithm>
#include <bit>

export module geometry_lib: bvh;

import :bvh_imp;

import matrix_math;

namespace geometry
{

	export using geometry::node;
	export using geometry::node_type;

	export 
	class bvh
	{
	public:

		template<typename vec>
		void build(std::span<const vec> rep_points)
		{
			imp.build(rep_points);
		}

		void traverse_bottom_up(const node& p_node, std::function<bool(const node& curr, const node& left, const node& right)> node_fn) const 
		{
			imp.traverse_bottom_up(p_node, node_fn);
		}

		void traverse_top_down(const node& p_node, std::function<bool(const node& curr, const node& left, const node& right)> node_fn) const 
		{
			imp.traverse_top_down(p_node, node_fn);
		}

	private:
		bvh_imp imp;
	};


	export 
	template<typename AABB>
	class bvh_traverser
	{
	public:
		void traverse_top_down(const bvh& p_bvh, std::span<const AABB> aabbs, std::function<bool(const AABB&)> internal_fn, std::function<void(int)> leaf_fn, bool update_aabb = true)
		{
			if (update_aabb)
			{
				update_iternal_box(p_bvh, aabbs);
			}

			auto get_aabb = [&](const auto& n)
				{
					if (n.type == geometry::node_type::leaf)
					{
						return aabbs[n.index];
					}
					else
					{
						return m_internal_AABB[n.index];
					}
				};

			auto culled_fn = [&](const auto& curr, const auto& l, const auto& r)
				{
					if (curr.type == node_type::inner)
					{
						return internal_fn(get_aabb(curr));
					}
					else
					{
						if (internal_fn(get_aabb(curr)))
						{
							leaf_fn(curr.index);
						}
						return false;
					}
				};

			p_bvh.traverse_top_down({ 0,node_type::inner }, culled_fn);
		}

		const std::vector<AABB>& get_internal_AABB() const
		{
			return m_internal_AABB;
		}
	private:

		void update_iternal_box(const bvh& p_bvh, std::span<const AABB> aabbs)
		{

			std::vector<bool> is_accessed(aabbs.size() - 1);
			m_internal_AABB.resize(aabbs.size() - 1);

			auto get_aabb = [&](const auto& n)
				{
					if (n.type == geometry::node_type::leaf)
					{
						return aabbs[n.index];
					}
					else
					{
						return m_internal_AABB[n.index];
					}
				};

			auto is_handled = [&](const auto& p_node)
				{
					return (p_node.type == geometry::node_type::inner && is_accessed[p_node.index])
						|| p_node.type == geometry::node_type::leaf;

				};

			auto build_iternal_aabb = [&](const auto& curr, const auto& l, const auto& r)
				{
					if (curr.type == geometry::node_type::inner)
					{
						if ((!is_handled(curr)) && is_handled(l) && is_handled(r))
						{
							m_internal_AABB[curr.index] = get_aabb(l) + get_aabb(r);
							is_accessed[curr.index] = true;
						}
						else
						{
							return false;
						}

					}

					return true;
				};

			for (int i = 0; i < aabbs.size() - 1; i++)
			{
				p_bvh.traverse_bottom_up({ i,geometry::node_type::inner }, build_iternal_aabb);
			}

			m_internal_AABB.shrink_to_fit();
		}

	private:

		std::vector<AABB> m_internal_AABB;
	};




	export
	template<typename vec>
	class linear_binary_radix_tree
	{
	public:
		enum node_type : int
		{
			inner = 0,
			leaf
		};

		struct node
		{
			int index = -1;
			node_type  type;
		};

	public:
		void build(std::span<const vec> rep_points)
		{
			compute_order_key(m_rep_pos_with_key, rep_points);

			sort_by_key(m_rep_pos_with_key);

			compute_range_and_aasign_children(m_nodes);
		}

		using key_type = uint32_t;

		void set_order_key_function(std::function<key_type(vec)> order_key_fn)
		{
			m_order_key_fn = order_key_fn;
		}

	public:
		int get_leaf_size() const
		{
			return m_rep_pos_with_key.size();
		}

		node get_parent(const node& n) const
		{
			return m_nodes[n.type][n.index].parent;
		}

		node get_left(const node& n) const
		{
			return m_nodes[n.type][n.index].left;
		}

		node get_right(const node& n) const
		{
			return m_nodes[n.type][n.index].right;
		}

		node get_root() const
		{
			return { .index = 0,.type = node_type::inner };
		}

		bool node_is_valid(const node& n)
		{
			return n.index != -1;
		}


	private:
		struct key_index
		{
			key_type key;
			int		index;
		};

		struct range
		{
			int begin;
			int end;
		};

		struct node_connect
		{
			node left;
			node right;
			node parent;
		};

	private:

		void compute_order_key(std::vector<key_index>& records, std::span<const vec> rep_points)
		{
			records.resize(rep_points.size());
			for (int i = 0; i < rep_points.size(); i++)
			{
				auto key = m_order_key_fn(rep_points[i]);

				records[i] = { .key = key,.index = i };
			}
		}

		void sort_by_key(std::vector<key_index>& records)
		{
			std::sort(std::begin(records), std::end(records), [](const auto& l, const auto& r) { return l.key < r.key; });
		}

		void compute_range_and_aasign_children(std::array<std::vector<node_connect>, 2>& nodes)
		{
			int leaf_num = m_rep_pos_with_key.size();

			int iternal_node_num = leaf_num - 1;

			if (iternal_node_num <= 0)
			{
				return;
			}

			nodes[node_type::inner].resize(iternal_node_num);
			nodes[node_type::leaf].resize(leaf_num);

			nodes[node_type::inner][0].parent = { .index = -1 };

			for (int i = 0; i < iternal_node_num; i++)
			{
				//range rng = determin_range(m_rep_pos_with_key[i].index );
				range rng = determin_range(i);

				int split = find_split(rng);

				node_type left_node_type = split == rng.begin ? node_type::leaf : node_type::inner;
				node_type right_node_type = split + 1 == rng.end ? node_type::leaf : node_type::inner;

				nodes[node_type::inner][i].left = { .index = split,.type = left_node_type };
				nodes[node_type::inner][i].right = { .index = split + 1,.type = right_node_type };

				nodes[left_node_type][split].parent = { .index = i,.type = node_type::inner };
				nodes[right_node_type][split + 1].parent = { .index = i,.type = node_type::inner };

			}
		}
		
		int common_prefix_num(int curr_index, int other_index)
		{
			if (other_index < m_rep_pos_with_key.size() && other_index >= 0)
			{
				return std::countl_zero(m_rep_pos_with_key[curr_index].key ^ m_rep_pos_with_key[other_index].key);
			}
			else
			{
				return -1;
			}
		}

		int sign(int v)
		{
			return v > 0 ? 1 : -1;
		}

		int line_search(int num, std::function<bool(int)> predicat)
		{
			int step = 0;

			int  upper_bound = num;

			while (true)
			{
				if (upper_bound == 1)
				{
					break;
				}

				upper_bound /= 2;

				int trial_step = step + upper_bound;

				if (predicat(trial_step))
				{
					step = trial_step;
				}

			}
			return step;

		}

		range determin_range(int current_index)
		{
			int dir = sign(common_prefix_num(current_index, current_index + 1) - common_prefix_num(current_index, current_index - 1));

			int common_prefix_sibling = common_prefix_num(current_index, current_index - dir);

			int upper_bound = 2;

			while (true)
			{
				int searched_index = current_index + upper_bound * dir;

				if (common_prefix_num(current_index, searched_index) <= common_prefix_sibling)
				{
					break;
				}

				upper_bound *= 2;
			}

			int range_of_other_side = line_search(upper_bound,
				[&](int trial_range_of_other_side)
				{
					int searched_index = current_index + trial_range_of_other_side * dir;
					return common_prefix_num(current_index, searched_index) > common_prefix_sibling;
				}
			);

			int r0 = current_index;
			int r1 = current_index + range_of_other_side * dir;
			return { .begin = std::min(r0,r1),.end = std::max(r0,r1) };
		}

		int find_split(const range& rng)
		{
			int common_prefix_of_range = common_prefix_num(rng.begin, rng.end);

			int split = line_search(rng.end - rng.begin,
				[&](int trial_split)
				{
					int searched_index = rng.begin + trial_split;
					return common_prefix_num(rng.begin, searched_index) > common_prefix_of_range;
				}
			);

			return rng.begin + split;
		}
		
	private:

		std::function<key_type(vec)>	m_order_key_fn;
		std::vector<key_index>			m_rep_pos_with_key;

		std::array<std::vector<node_connect>, 2> m_nodes; //0 for internal, 1 for leaf

	};

	export
	template<typename vec,typename AABB>
	class LBVH
	{

	public:
		using node_type = linear_binary_radix_tree<vec>::node_type;
		using node = linear_binary_radix_tree<vec>::node;
		using key_type = linear_binary_radix_tree<vec>::key_type;


		void build(std::span<const vec> rep_points, std::function<key_type(vec)> order_key_fn)
		{
			m_tree.set_order_key_function(order_key_fn);
			m_tree.build(rep_points);
		}


		void assign_leaf_aabb(std::span<const AABB> aabbs)
		{
			int leaf_size = m_tree.get_leaf_size();

			m_leaf_aabbs = aabbs;
			m_inner_aabbs.resize(leaf_size - 1);

			m_aabbs[node_type::inner] = m_inner_aabbs.data();
			m_aabbs[node_type::leaf] = m_leaf_aabbs.data();

			std::vector<bool> inner_node_has_been_visited(leaf_size - 1, false);

			for (int i = 0; i < leaf_size; i++)
			{
				node curr_node{ .index = i,.type = node_type::leaf };

				while (true)
				{
					curr_node = m_tree.get_parent(curr_node);

					if (!m_tree.node_is_valid(curr_node))
					{
						break;
					}

					if (inner_node_has_been_visited[curr_node.index])
					{
						auto left = get_left(curr_node);
						auto right = get_right(curr_node);
						//assert: curr_node.type == node_type::inner
						m_inner_aabbs[curr_node.index] = get_aabb(left) + get_aabb(right);
					}
					else
					{
						inner_node_has_been_visited[curr_node.index] = true;
					}
				}

			}
		}


		node get_root() const
		{
			return m_tree.get_root();
		}


		node get_left(const node& n) const
		{
			return m_tree.get_left(n);
		}

		node get_right(const node& n) const
		{
			return m_tree.get_right(n);
		}


		const AABB& get_aabb(const node& n) const
		{
			return	m_aabbs[n.type][n.index];
		}


	private:
		std::vector<AABB> m_inner_aabbs;
		std::span<const AABB> m_leaf_aabbs;
		std::array<const AABB*, 2> m_aabbs; //0 for inner, 1 for leaf

		linear_binary_radix_tree<vec> m_tree;
	};

	export
	template<typename vec, typename AABB>
	class loop_traverser
	{
	public:
		using node_type = linear_binary_radix_tree<vec>::node_type;
		using node = linear_binary_radix_tree<vec>::node;

		struct compare_pair
		{
			int leafI;
			int loopI;
		};
	public:
		void loop_traverse(int num, const LBVH<vec,AABB>& bvh, std::function<bool(const AABB&, int)> inner_fn, std::function<void(compare_pair)> leaf_fn)
		{
			for (int i = 0; i < num; i++)
			{
				auto inner_i_fn = [&, i](const AABB& aabb) { return inner_fn(aabb, i); };
				auto leaf_i_fn = [&, i](int leafI) { return leaf_fn({ .leafI = leafI,.loopI = i }); };

				traverse_from_root(bvh, inner_i_fn, leaf_i_fn);
			}
		}

		void traverse_from_root(const LBVH<vec, AABB>& bvh, std::function<bool(const AABB&)> inner_fn, std::function<void(int)> leaf_fn)
		{
			std::vector<node> node_stack;
			node_stack.push_back(bvh.get_root());

			while (!node_stack.empty())
			{
				auto curr_node = node_stack.back();
				node_stack.pop_back();

				if (curr_node.type == node_type::leaf)
				{
					leaf_fn(curr_node.index);
				}
				else
				{
					if (inner_fn(bvh.get_aabb(curr_node)))
					{
						node_stack.push_back(bvh.get_left(curr_node));
						node_stack.push_back(bvh.get_right(curr_node));
					}
				}

			}

		}

	};


}