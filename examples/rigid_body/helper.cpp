
module;

#include <vector> 
#include <array> 

module rigid_body_dynamic:helper;

import matrix_math;

template<typename T, int N>
const T* as_buffer(const std::vector<matrix_math::matrix<T, N, 1>>& in)
{
	return &(in.front()(0));
}

template<typename T, int N>
std::array<T, N> vec_2_arr(const matrix_math::matrix<T, N, 1>& in)
{
	std::array<T, N> ret;
	for (int i = 0; i < N; i++)
	{
		ret[i] = in(i);
	}
	return ret;
}

template<typename T, int N>
matrix_math::matrix<T, N, 1> arr_2_vec(const std::array<T, N>& in)
{
	matrix_math::matrix<T, N, 1> ret;
	for (int i = 0; i < N; i++)
	{
		ret(i) = in[i];
	}
	return ret;
}
