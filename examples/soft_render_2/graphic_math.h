
#pragma once

#include "mat.h"

namespace soft_render
{

	template<typename T ,int N>
	static mat<T, N + 1, 1> get_homogeneous_point(const mat<T, N, 1>& v)
	{
		mat<T, N + 1, 1> ret;
		for (int i = 0; i < N; i++)
		{
			ret(i) = v(i);
		}
		ret(N) = 1.f;
		return ret;
	}

	template<typename T ,int N>
	static mat<T, N - 1, 1> from_homogeneous_point(const mat<T, N, 1>& v)
	{
		mat<T, N - 1, 1> ret;
		for (int i = 0; i < N - 1; i++)
		{
			ret(i) = v(i) / v(N - 1);
		}
		return ret;
	}

	template<typename T ,int N>
	static mat<T, N + 1, 1> get_homogeneous_vector(const mat<T, N, 1>& v)
	{
		mat<T, N + 1, 1> ret;
		for (int i = 0; i < N; i++)
		{
			ret(i) = v(i);
		}
		ret(N) = 0.f;
		return ret;
	}

	template<typename T ,int N>
	static mat<T, N - 1, 1> from_homogeneous_vector(const mat<T, N, 1>& v)
	{
		mat<T, N - 1, 1> ret;
		for (int i = 0; i < N - 1; i++)
		{
			ret(i) = v(i);
		}
		return ret;
	}


}
