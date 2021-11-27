

#pragma once
#include "matrix_math/matrix_math.h"
#include <numeric>

using namespace matrix_math;

template<int dim, typename T = float>
struct AABB
{
	AABB() 
		:
		m_lower(get_uniform<dim, 1, T>(std::numeric_limits<T>::max())),
		m_upper(get_uniform<dim, 1, T>(std::numeric_limits<T>::lowest()))
	{}

	AABB& operator+=(const vec<dim,T>& point)
	{
		for (int i = 0; i < dim; i++)
		{
			if (m_lower(i) > point(i))
			{
				m_lower(i) = point(i);
			}
			if (m_upper(i) < point(i))
			{
				m_upper(i) = point(i);
			}
		}
		return *this;
	}

	vec<dim, T>  m_lower;
	vec<dim, T>  m_upper;

};