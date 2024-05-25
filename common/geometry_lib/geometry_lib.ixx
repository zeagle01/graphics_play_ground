module;

#include <array>

export module  geometry_lib;


export import : bvh;
export import : aabb;

import matrix_math;


namespace geometry
{
	using mat3x3f = matrix_math::matrix<float, 3, 3>;
	using mat3x2f = matrix_math::matrix<float, 3, 2>;
	using vec3f = matrix_math::matrix<float, 3, 1>;

	export
	bool is_ray_triangle_intersect(vec3f& w, const mat3x3f& tX, const vec3f& p, const vec3f& dir)
	{
		constexpr mat3x2f S({
			1.f,0.f,
			0.f,1.f,
			-1.f,-1.f
			});
		
		w = (p - matrix_math::column(tX, 2)) / matrix_math::from_columns(tX * S, -dir);

		return w(0) > 0.f && w(0) < 1.f
			&& w(1) > 0.f && w(1) < 1.f
			&& w(2) > 0.f;
	}

}