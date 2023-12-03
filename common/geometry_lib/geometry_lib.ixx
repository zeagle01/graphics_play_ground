module;

#include <array>
#include "matrix_math/matrix_math.h"

export module  geometry_lib;


export import : bvh;

namespace geometry
{

	export
	bool is_ray_triangle_intersect(vec3f& w, const std::array<vec3f, 3>& tX, const vec3f& p, const vec3f& dir)
	{
		auto v0 = tX[0] - tX[2];
		auto v1 = tX[1] - tX[2];
		auto vp = p - tX[2];

		
		mat3x3f m; //{ v0, v1, -dir };
		m(0, 0) = v0(0);
		m(1, 0) = v0(1);
		m(2, 0) = v0(2);

		m(0, 1) = v1(0);
		m(1, 1) = v1(1);
		m(2, 1) = v1(2);

		m(0, 2) = -dir(0);
		m(1, 2) = -dir(1);
		m(2, 2) = -dir(2);

		w = vp / m;

		return w(0) > 0.f && w(0) < 1.f
			&& w(1) > 0.f && w(1) < 1.f
			&& w(2) > 0.f;
	}

}