

module;

#include "matrix_math/matrix_math.h"
#include <span>

export module geometry_lib: bvh;

import :bvh_imp;

import matrix_math;

namespace geometry
{
	export 
	template<typename vec>
	class bvh
	{
	public:
		void build(std::span<const vec> rep_points)
		{
			imp.build(rep_points);
		}
		//getters
	public:
		int parent(int leafNode) const
		{
			return 0;
		}

		int left_child(int node) const
		{
			return 0;
		}

		int right_child(int node) const
		{
			return 0;
		}

	private:
		bvh_imp<vec> imp;
	};


	export 
	class bvh_traverser
	{
	public:

		template<typename vec >
		void operator()(const bvh<vec>& bvh, std::function<bool(int node)> is_intersect)
		{
			int root = 0;
			std::vector<int> stack{ root };
			while ( !stack.empty())
			{
				int current_node = stack.back();
				stack.pop_back();
				if (is_intersect(current_node))
				{
					stack.push_back(bvh.left_child(current_node));
					stack.push_back(bvh.right_child(current_node));
				}
			}

		}
		

	};

}