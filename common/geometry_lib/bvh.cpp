
module;

#include <span>
#include <array>
#include <functional>

module geometry_lib: bvh_imp;

import matrix_math;


namespace geometry
{

	enum class node_type
	{
		leaf = 0,
		inner = 1
	};

	struct node
	{
		int index;
		node_type type;
		friend auto operator<=>(const node& l, const node& r) = default;
	};

	class bvh_imp
	{
	public:

		template<typename vec>
		void build(std::span<const vec> rep_points)
		{
			if (rep_points.empty())
			{
				return;
			}

			m_parent[int(node_type::inner)].assign(rep_points.size() - 1, -1);
			m_parent[int(node_type::leaf)].assign(rep_points.size(), -1);

			auto root = new_a_internal_node();
			range whole = { 0, int(rep_points.size()) - 1 };
			std::vector<tree_edge> edge_stack{ {-1,0,root,whole} };

			while (!edge_stack.empty())
			{
				auto current_edge = edge_stack.back();
				edge_stack.pop_back();

				if (current_edge.parent!= -1)
				{
					assign_child(current_edge.parent, current_edge.left_right, current_edge.me);
					assign_parent(current_edge.me, current_edge.parent);
				}

				if (is_inner_node(current_edge.me_range))
				{
					int mid = split(current_edge.me_range);
					std::array<range, 2> children_range = 
					{{
						{current_edge.me_range.begin,mid},
						{mid + 1,current_edge.me_range.end}
					}};

					for (int child_i = 0; child_i < 2; child_i++)
					{
						node new_node = new_a_internal_node(children_range[child_i]);

						edge_stack.push_back({ current_edge.me.index, child_i,new_node,children_range[child_i]});
					}

				}
			}
			m_internal_children.shrink_to_fit();
			m_parent[int(node_type::inner)].shrink_to_fit();
			m_parent[int(node_type::leaf)].shrink_to_fit();
		}

		void traverse_bottom_up(const node& p_node, std::function<bool(const node& curr, const node& left, const node& right)> node_fn) const
		{
			node curr = p_node;
			while (curr.index != -1)
			{
				bool go_up=node_fn(curr, left_child(curr), right_child(curr));

				if (!go_up)
				{
					break;
				}
				curr = parent(curr);
			}
		}

		void traverse_top_down(const node& p_node, std::function<bool(const node& curr, const node& left, const node& right)> node_fn) const
		{
			std::vector<node> stack{ p_node };
			while (!stack.empty())
			{
				node curr = stack.back();
				stack.pop_back();

				if (curr.index != -1)
				{
					bool go_down = node_fn(curr, left_child(curr), right_child(curr));

					if (!go_down)
					{
						continue;
					}

					stack.push_back(left_child(curr));
					stack.push_back(right_child(curr));
				}

			}

		}


	private:

		struct range
		{
			int begin;
			int end;
		};

		struct tree_edge
		{
			int parent;
			int left_right;
			node me;
			range me_range;
		};

		bool is_inner_node(range p_range)
		{
			return p_range.begin != p_range.end;
		}

		node parent(const node& n) const
		{
			return { m_parent[int(n.type)][n.index], node_type::inner };
		}

		node left_child(const node& n) const
		{
			if (n.type == node_type::leaf)
			{
				return { -1 };
			}
			else
			{
				return m_internal_children[n.index][0];
			}
		}

		node right_child(const node& n) const
		{
			if (n.type == node_type::leaf)
			{
				return { -1 };
			}
			else
			{
				return m_internal_children[n.index][1];
			}
		}

		void assign_child(int n, int left_right, node child)
		{
			m_internal_children[n][left_right] = child;
		}
		void assign_parent(node n, int parent)
		{
			m_parent[int(n.type)][n.index] = parent;
		}

		node new_a_internal_node(range p_range)
		{
			if (is_inner_node(p_range))
			{
				return new_a_internal_node();
			}
			else
			{
				return { p_range.begin,node_type::leaf };
			}
		}

		node new_a_internal_node()
		{
			int new_internal_node = m_internal_children.size();
			children c;
			c[0].index = -1;
			c[1].index = -1;
			m_internal_children.push_back(c);
			return { new_internal_node,node_type::inner };
		}

		int  split(range p_range)
		{
			return  (p_range.begin + p_range.end) / 2;
		}


		using children = std::array<node, 2>;
		std::vector<children> m_internal_children;
		std::array<std::vector<int>, 2> m_parent;
	};


	//template<typename AABB>
	//class bvh_traverser_imp
	//{
	//public:

	//	void update_iternal_box(const bvh& p_bvh, std::span<const AABB> boxes)
	//	{
	//	}

	//	
	//};
}
