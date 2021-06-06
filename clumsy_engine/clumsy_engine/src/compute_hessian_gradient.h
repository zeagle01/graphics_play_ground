
#pragma once

#include <vector>
#include "matrix_math/matrix_math.h"


struct  Compute_Hessian_Gradient
{
	template<int vertex_of_stencel_num>
	static void apply(
		std::vector<mat3x3f>& A, std::vector<vec3f>& b,
		const vec<vertex_of_stencel_num,float>& weight,
		const vec3f& constraint_normal,
		float violation_distance,
		float stiff )
	{
		A.resize(vertex_of_stencel_num * vertex_of_stencel_num);
		b.resize(vertex_of_stencel_num );
		for (int i = 0; i < vertex_of_stencel_num; i++)
		{
			for (int j = 0; j < vertex_of_stencel_num; j++)
			{
				A[i * vertex_of_stencel_num + j] = stiff * weight(i) * weight(j) * constraint_normal ^ constraint_normal;
			}
		}

		for (int i = 0; i < vertex_of_stencel_num; i++)
		{
			b[i] = stiff * violation_distance * constraint_normal;
		}
	}
};
