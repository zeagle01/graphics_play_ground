module;

#include <array>

export module  geometry_lib;


export import : bvh;
export import : aabb;

import matrix_math;


namespace geometry
{
	using mat3x3f = matrix_math::matrix<float, 3, 3>;
	using vec3f = matrix_math::matrix<float, 3, 1>;

	export
	bool is_ray_triangle_intersect(vec3f& w, const std::array<vec3f, 3>& tX, const vec3f& p, const vec3f& dir)
	{
		auto v0 = tX[0] - tX[2];
		auto v1 = tX[1] - tX[2];
		auto vp = p - tX[2];

		
		mat3x3f m = matrix_math::from_columns(v0, v1, -dir); //{ v0, v1, -dir };

		w = vp / m;

		return w(0) > 0.f && w(0) < 1.f
			&& w(1) > 0.f && w(1) < 1.f
			&& w(2) > 0.f;
	}

}