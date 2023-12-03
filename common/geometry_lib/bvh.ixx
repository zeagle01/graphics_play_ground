

module;

#include <span>
#include "matrix_math/matrix_math.h"

export module geometry_lib: bvh;

import :bvh_imp;

import matrix_math;

namespace geometry
{
	export class bvh
	{
	public:
		void build(std::span<const vec3f> rep_points)
		{
			imp.build(rep_points);
		}

		void update_aabb(std::span<const vec3f> aabbs);
	private:
		bvh_imp imp;
	};


	export class bvh_traverser
	{
		

	};

}