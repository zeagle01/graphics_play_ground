

module;

#include "matrix_math/matrix_math.h"
#include <span>

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


		void traverse_top_down(const AABB& p_AABB, const bvh& p_bvh, std::span<const AABB> aabbs, std::function<bool(const node& curr, const node& left, const node& right,bool )> node_fn)
		{
			update_iternal_box(p_bvh, aabbs);

			auto culled_fn = [&](const auto& curr, const auto& l, const auto& r)
				{
					bool intersected = p_AABB.intersect(aabbs[curr.index]);
					bool keep_going = node_fn(curr, l, r, intersected);
					return keep_going && intersected;
				};
			p_bvh.traverse_top_down({ 0,node_type::inner }, culled_fn);
		}

		const std::vector<AABB>& get_internal_AABB() const
		{
			return m_internal_AABB;
		}

	private:
		std::vector<AABB> m_internal_AABB;
		
	};

}