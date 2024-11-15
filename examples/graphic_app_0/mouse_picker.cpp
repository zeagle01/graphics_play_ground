

#include "mouse_picker.h"

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

import geometry_lib;
import matrix_math;


using vec3f = matrix_math::matrix<float, 3, 1>;
using mat3x3f = matrix_math::matrix<float, 3, 3>;

template<typename T,int N>
using vec_tn = matrix_math::matrix<T, N, 1>;


using AABB = geometry::AABB3f<vec_tn>;

picked_info mouse_picker::pick(
	const std::array<float, 3>& p, const std::array<float, 3>& dir,
	const float* x, const int* indices, int vNum, int tNum )
{
	bool use_bvh = true;
	if (use_bvh)
	{
		return bvh_pick(p, dir, x, indices, vNum, tNum);
	}
	else
	{
		return loop_pick(p, dir, x, indices, vNum, tNum);
	}

}


picked_info mouse_picker::bvh_pick(const std::array<float, 3>& p, const std::array<float, 3>& dir,
	const float* x, const int* indices, int vNum, int tNum)
{
	std::vector<vec3f> rep_points(tNum);
	std::vector<AABB> t_AABBs(tNum);
	AABB total_box;
	for (int ti = 0; ti < tNum; ti++)
	{
		for (int vi = 0; vi < 3; vi++)
		{
			int v = indices[ti * 3 + vi];
			auto p = vec3f({ x[v * 3 + 0],x[v * 3 + 1],x[v * 3 + 2] });
			t_AABBs[ti] += p;

			rep_points[ti] += p / 3;
		}
		total_box += t_AABBs[ti];
	}
	auto l = total_box.get_lower();
	auto u = total_box.get_upper();
	for (int ti = 0; ti < tNum; ti++)
	{
		for (int di = 0; di < 3; di++)
		{
			rep_points[ti](di) = (rep_points[ti](di) - l(di)) / std::max(u(di) - l(di), 1e-3f);
		}
	}


	AABB picked_box;
	picked_box += vec3f({ p[0],p[1],p[2] });


	geometry::LBVH<vec3f, AABB> l_bvh;
	geometry::z_order_key z_order;
	l_bvh.build(rep_points, [&](const auto& v) { return z_order(v(0), v(1), v(2)); });
	l_bvh.assign_leaf_aabb(t_AABBs);
	geometry::loop_traverser<vec3f, AABB> traverser;


	vec3f w;
	auto pTemp = vec3f({ p[0],p[1],p[2] });
	auto dirTemp = vec3f({ dir[0], dir[1], dir[2] });
	picked_info ret = { -1,{} };
	traverser.traverse_from_root(l_bvh, 

		[&](const auto& node_AABB)
		{
			return picked_box.intersect(node_AABB);
		},

		[&](int i)
		{
			std::array<int, 3> t = { indices[i * 3 + 0],indices[i * 3 + 1],indices[i * 3 + 2] };
			mat3x3f X 
			( {
				x[t[0] * 3 + 0],     x[t[1] * 3 + 0],     x[t[2] * 3 + 0],
				x[t[0] * 3 + 1],     x[t[1] * 3 + 1],     x[t[2] * 3 + 1],
				x[t[0] * 3 + 2],     x[t[1] * 3 + 2],     x[t[2] * 3 + 2]
			} );

			if (geometry::is_ray_triangle_intersect(w, X, pTemp, dirTemp))
			{
				auto hitTemp = pTemp + w(2) * dirTemp;
				ret = { i,{w(0),w(1),1 - w(0) - w(1)} };
			}
		}
	);
	return ret;

}

picked_info mouse_picker::loop_pick(const std::array<float, 3>& p, const std::array<float, 3>& dir,
	const float* x, const int* indices, int vNum, int tNum)
{
	for (int i = 0; i < tNum; i++)
	{
		
		std::array<int, 3> t = { indices[i * 3 + 0],indices[i * 3 + 1],indices[i * 3 + 2] };

		mat3x3f X
		({
			x[t[0] * 3 + 0],     x[t[1] * 3 + 0],     x[t[2] * 3 + 0],
			x[t[0] * 3 + 1],     x[t[1] * 3 + 1],     x[t[2] * 3 + 1],
			x[t[0] * 3 + 2],     x[t[1] * 3 + 2],     x[t[2] * 3 + 2]
			});

		vec3f w;
		auto pTemp = vec3f({ p[0],p[1],p[2] });
		auto dirTemp = vec3f({ dir[0], dir[1], dir[2] });
		if (geometry::is_ray_triangle_intersect(w, X, pTemp, dirTemp))
		{
			auto hitTemp = pTemp + w(2) * dirTemp;
			return { i,{w(0),w(1),1 - w(0) - w(1)} };
		}
	}

	return { -1,{} };

}
