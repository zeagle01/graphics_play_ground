
#pragma once

#include <cmath>
#include <vector>
#include <array>
#include <iostream>

namespace soft_render
{
	template<typename T, int Row, int Col>
	struct mat
	{
		//std::array< T, Row* Col> data;
		T data[Row * Col];
		T& operator()(int ri, int ci) { return data[ri * Col + ci]; }
		const T& operator() (int ri, int ci) const { return data[ri * Col + ci]; }

		T& operator()(int i) { return data[i]; }
		const T& operator() (int i) const { return data[i]; }

	};

	using mat4 = mat<float, 4, 4>;
	using mat3 = mat<float, 3, 3>;
	using mat2 = mat<float, 2, 2>;
	using vec3 = mat<float, 3, 1>;
	using vec2 = mat<float, 2, 1>;
	using vec4 = mat<float, 4, 1>;

	using vec2i = mat<int,2, 1>;
	using vec3i = mat<int,3, 1>;
	using vec4i = mat<int,4, 1>;



	template<typename T,int Row, int N, int Col>
	static mat<T,Row, Col> operator*(const mat<T,Row, N>& a, const mat<T,N, Col>& b)
	{
		mat<T,Row, Col> ret;
		for (int ri = 0; ri < Row; ri++)
		{
			for (int ci = 0; ci < Col; ci++)
			{
				ret(ri, ci) = 0.f;
				for (int li = 0; li < N; li++)
				{
					ret(ri, ci) += a(ri, li) * b(li, ci);
				}
			}
		}
		return ret;
	}

	template<typename T, int Row, int Col>
	static std::ostream& operator<<(std::ostream& out, const mat<T,Row, Col>& m)
	{
		for (int i = 0; i < Row * Col; i++)
		{
			out << m.data[i]<<" ";
		}
		return out;
	}

	template<typename T, int Row, int Col>
	static std::istream& operator>>(std::istream& in,  mat<T,Row, Col>& m)
	{
		for (int i = 0; i < Row * Col; i++)
		{
			in >> m.data[i];
		}
		return in;
	}


	template<typename T, int Row, int Col>
	static mat<T, Row, Col> operator+(const mat<T, Row, Col>& a, const mat<T, Row, Col>& b)
	{
		mat<T,Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = a.data[i] + b.data[i];
		}
		return ret;
	}

	template<typename T, int Row, int Col>
	static mat<T, Row, Col> operator+(const T& s, const mat<T, Row, Col>& a)
	{
		mat<T,Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = a.data[i] + s;
		}
		return ret;
	}

	template<typename T,int Row, int Col>
	static mat<T, Row, Col> operator-(const mat<T, Row, Col>& a, const mat<T, Row, Col>& b)
	{
		mat<T,Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = a.data[i] - b.data[i];
		}
		return ret;
	}

	template<typename T,int Row, int Col>
	static mat<T, Row, Col> operator-(const mat<T, Row, Col>& a)
	{
		mat<T,Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = -a.data[i];
		}
		return ret;
	}

	template<typename T, int Row, int Col>
	static mat<T,Row, Col> operator*(T s, const mat<T,Row, Col>& a)
	{
		mat<T,Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = s * a.data[i];
		}
		return ret;
	}

	template<typename  T, int Row, int Col>
	static mat<T,Row, Col> operator/(const mat<T,Row, Col>& a, T s)
	{
		mat<T,Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = a.data[i] / s;
		}
		return ret;
	}

	static vec3 cross(const vec3& a, const vec3& b)
	{
		vec3 ret;
		ret.data[0] = a.data[1] * b.data[2] - a.data[2] * b.data[1];
		ret.data[1] = -a.data[0] * b.data[2] + a.data[2] * b.data[0];
		ret.data[2] = a.data[0] * b.data[1] - a.data[1] * b.data[0];
		return ret;
	}

	template<typename T,int N>
	static T dot(const mat<T, N, 1>& a, const mat<T, N, 1>& b)
	{
		T ret = 0.f;
		for (int i = 0; i < N; i++)
		{
			ret += a.data[i] * b.data[i];
		}
		return ret;
	}

	template<typename T, int N>
	static T length(const mat<T,N, 1>& v)
	{
		return std::sqrt(dot(v, v));
	}

	template<typename T, int N>
	static mat<T, N, 1> normalize(const mat<T, N, 1>& v)
	{
		return v / length(v);
	}

	template<typename T, int N>
	static mat<T, N, N> get_identity()
	{
		mat<T,N, N> ret;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (i == j)
				{
					ret(i, j) = 1.f;
				}
				else
				{
					ret(i, j) = 0.f;
				}
			}
		}
		return ret;
	}

	template<typename T, int N>
	static mat<T, N, 1> GS_solve(const mat<T, N, N>& A, const mat<T, N, 1>& b)
	{
		auto temp_b = b;
		auto U = A;
		for (int round = 0; round < N; round++)
		{
			for (int row = round + 1; row < N; row++)
			{
				float c = -U(row, round) / U(round, round);
				for (int col = round ; col < N; col++)
				{
					U(row, col) += c * U(round, col);
				}
				temp_b(row) += c * temp_b(round);
			}
		}

		mat<T, N, 1> ret;
		for (int row = N - 1; row >= 0; row--)
		{
			ret(row) = temp_b(row);
			for (int col = row + 1; col < N; col++)
			{
				ret(row) -= U(row, col) * ret(col);
			}
			ret(row) /= U(row, row);
		}

		return ret;
	}

}

