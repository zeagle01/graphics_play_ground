

#include "mouse_picker.h"
#include "matrix_math/matrix_math.h"

#include <iostream>
#include <array>

import geometry_lib;

picked_info mouse_picker::pick(
	const std::array<float, 3>& p, const std::array<float, 3>& dir,
	const float* x, const int* indices, int vNum, int tNum
	)
{

//	//TODO: use bvh 
//	geometry::bvh bvh;
//	std::vector<vec3f> rep_points;
//	bvh.build(rep_points);
//	geometry::bvh_traverser tr;


	//using vec = std::array<float, 3>;
	for (int i = 0; i < tNum; i++)
	{
		
		std::array<int, 3> t = { indices[i * 3 + 0],indices[i * 3 + 1],indices[i * 3 + 2] };

		std::array<vec3f, 3> X
		{{
			{x[t[0] * 3 + 0],x[t[0] * 3 + 1],x[t[0] * 3 + 2]},
			{x[t[1] * 3 + 0],x[t[1] * 3 + 1],x[t[1] * 3 + 2]},
			{x[t[2] * 3 + 0],x[t[2] * 3 + 1],x[t[2] * 3 + 2]},
		}};

		vec3f w;
		auto pTemp= vec3f{ p[0],p[1],p[2] };
		auto dirTemp = vec3f{ dir[0],dir[1],dir[2] };
		if (geometry::is_ray_triangle_intersect(w, X, pTemp,dirTemp ))
		{
			auto hitTemp = pTemp + w(2) * dirTemp;
			return { i,{w(0),w(1),1 - w(0) - w(1)} };
		}
	}

	return { -1,{} };
;
}

