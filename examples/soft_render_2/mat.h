
#pragma once

#include <cmath>
#include <vector>
#include <array>

namespace soft_render
{
	template<int Row, int Col>
	struct mat
	{
		//std::array< float, Row* Col> data;
		float data[Row * Col];

		float& operator()(int ri, int ci) { return data[ci * Row + ri]; }
		const float& operator() (int ri, int ci) const { return data[ci * Row + ri]; }

		float& operator()(int i) { return data[i]; }
		const float& operator() (int i) const { return data[i]; }

	};


	using mat4 = mat<4, 4>;
	using vec3 = mat<3, 1>;
	using vec4 = mat<4, 1>;


	template<int Row, int N, int Col>
	static mat<Row, Col> operator*(const mat<Row, N>& a, const mat<N, Col>& b)
	{
		mat<Row, Col> ret;
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


	template<int Row, int Col>
	static mat<Row, Col> operator+(const mat<Row, Col>& a, const mat<Row, Col>& b)
	{
		mat<Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = a.data[i] + b.data[i];
		}
		return ret;
	}

	template<int Row, int Col>
	static mat<Row, Col> operator-(const mat<Row, Col>& a, const mat<Row, Col>& b)
	{
		mat<Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = a.data[i] - b.data[i];
		}
		return ret;
	}

	template<int Row, int Col>
	static mat<Row, Col> operator*(float s, const mat<Row, Col>& a)
	{
		mat<Row, Col> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			ret.data[i] = s * a.data[i];
		}
		return ret;
	}

	template<int Row, int Col>
	static mat<Row, Col> operator/(const mat<Row, Col>& a, float s)
	{
		mat<Row, Col> ret;
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

	template<int N>
	static float dot(const mat<N, 1>& a, const mat<N, 1>& b)
	{
		float ret;
		for (int i = 0; i < N; i++)
		{
			ret += a.data[i] * b.data[i];
		}
		return ret;
	}

	template<int N>
	static float length(const mat<N, 1>& v)
	{
		return std::sqrt(dot(v, v));
	}

	template<int N>
	static mat<N, 1> normalize(const mat<N, 1>& v)
	{
		return v / length(v);
	}

	template<int N>
	static mat<N, N> get_identity()
	{
		mat<N, N> ret;
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

}

