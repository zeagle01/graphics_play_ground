
#pragma once

#include <initializer_list>
#include <numeric>
#include <algorithm>
#include <limits>
#include <functional>

#ifdef __CUDACC__
#define DEVICE_CALLABLE __device__  __host__
#else
#define DEVICE_CALLABLE  
#endif

namespace matrix_math
{

////////////////////////////////////matrix data//////////////////////////////

#define MAT_DATA(Row,Col,T) \
	T data[Row * Col]; \
	DEVICE_CALLABLE const T& operator()(int ri, int ci) const { return data[ri + ci * Row]; }\
	DEVICE_CALLABLE T& operator()(int ri, int ci) { return data[ri + ci * Row]; } \
	DEVICE_CALLABLE const T* operator&() const {return data;} \
	DEVICE_CALLABLE T* operator&(){ return data;} \

#define VEC_ACCESS(T) \
		DEVICE_CALLABLE const T& operator()(int i) const { return data[i]; } \
		DEVICE_CALLABLE T& operator()(int i) { return data[i]; }  \


	template<int Row, int Col, typename T>
	struct mat  
	{
		MAT_DATA(Row,Col,T)
	};

	template<int Row,  typename T>
	struct mat<Row,1,T>
	{
		MAT_DATA(Row, 1, T)
		VEC_ACCESS(T)
	};

	template<int Col,  typename T>
	struct mat<1,Col,T>
	{
		MAT_DATA(1, Col, T)
		VEC_ACCESS(T)
	};

	template<typename T>
	struct mat<1,1,T>
	{
		MAT_DATA(1, 1, T)
		VEC_ACCESS(T)
		operator T() const { return data[0]; }
		operator T& () { return data[0]; }
	};


////////////////////////////////////operation //////////////////////////////
	template<typename T> struct Add { static DEVICE_CALLABLE T apply(T v0, T v1) { return v0 + v1; } };
	template<typename T> struct Substract { static DEVICE_CALLABLE T apply(T v0, T v1) { return v0 - v1; } };
	template<typename T> struct Multiply { static DEVICE_CALLABLE T apply(T v0, T v1) { return v0 * v1; } };
	template<typename T> struct Divide { static DEVICE_CALLABLE T apply(T v0, T v1) { return v0 / v1; } };

	template<typename T> struct Max { static DEVICE_CALLABLE T apply(T v0, T v1) { return v0 > v1 ? v0 : v1; } };
	template<typename T> struct Min { static DEVICE_CALLABLE T apply(T v0, T v1) { return v0 < v1 ? v0 : v1; } };


	template<typename T, int N, template<typename> typename Op>
	static DEVICE_CALLABLE inline void BinaryOp(T* a, const T* b, const T* c)
	{
		for (int i = 0; i < N; i++)
		{
			a[i] = Op<T>::apply(b[i], c[i]);
		}
	}

	template<typename T, int N, template<typename> typename Op,typename ...P>
	static DEVICE_CALLABLE inline void UnaryOp(T* a, const T* b, P&&... p)
	{
		for (int i = 0; i < N; i++)
		{
			a[i] = Op<T>::apply(b[i], std::forward<P>(p)...);
		}
	}


////////////////////////////////////init //////////////////////////////
	template<int N, typename T>
	static DEVICE_CALLABLE inline mat<N, N, T> get_identity()
	{
		mat<N, N, T> ret;
		for (int ri = 0; ri < N; ri++)
		{
			for (int ci = 0; ci < N; ci++)
			{
				if (ri == ci)
				{
					ret(ri, ci) = 1.f;
				}
				else
				{
					ret(ri, ci) = 0.f;
				}
			}
		}
		return ret;
	}

	template<int Row, int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> get_uniform(T value)
	{
		mat<Row, Col, T> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			(&ret)[i] = value;
		}
		return ret;
	}

////////////////////////////////////plus minus //////////////////////////////
#define COMPONENTWISE_BINARY(name,component_op) \
	template<int Row,int Col,typename T> \
	static DEVICE_CALLABLE inline mat<Row, Col, T> name(const mat<Row, Col, T>& a, const mat<Row, Col, T>& b)\
	{\
		mat<Row, Col, T> ret;\
		BinaryOp<T, Row* Col, component_op>(&ret, &a, &b);\
		return ret;\
	}\

#define COMPONENTWISE_SELF_BINARY(name,component_op) \
	template<int Row,int Col,typename T> \
	static DEVICE_CALLABLE inline mat<Row, Col, T>& name(mat<Row, Col, T>& a, const mat<Row, Col, T>& b)\
	{\
		BinaryOp<T, Row* Col, component_op>(&a, &a, &b);\
		return a;\
	}\

COMPONENTWISE_BINARY(operator+,Add)
COMPONENTWISE_BINARY(operator-,Substract)

COMPONENTWISE_SELF_BINARY(operator+=,Add)
COMPONENTWISE_SELF_BINARY(operator-=,Substract)


////////////////////////////////////the matrix product//////////////////////////////
	template<int Row, int N, int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> operator*(const mat<Row, N, T>& a, const mat<N, Col, T>& b)
	{
		mat<Row, Col, T> ret;
		for (int ri = 0; ri < Row; ri++)
		{
			for (int ci = 0; ci < Col; ci++)
			{
				ret(ri, ci) = 0.f;
				for (int ni = 0; ni < N; ni++)
				{
					ret(ri, ci) += a(ri, ni) * b(ni, ci);
				}
			}
		}
		return ret;
	}

////////////////////////////////////scalar product/divide//////////////////////////////
	template<int Row,int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> operator*(const mat<Row, Col, T>& a, const T& s)
	{
		mat<Row, Col, T> ret;
		UnaryOp<T, Row* Col, Multiply>(&ret, &a, s);
		return ret;
	}

	template<int Row,int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> operator*(const T& s, const mat<Row, Col, T>& a) { return a * s; }

	template<int Row,int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> operator/(const mat<Row, Col, T>& a, const T& s)
	{
		mat<Row, Col, T> ret;
		UnaryOp<T, Row* Col, Divide>(&ret, &a, s);
		return ret;
	}


////////////////////////////////////cross hadamar kronecker//////////////////////////////
	template< typename T>
	static DEVICE_CALLABLE inline mat<3, 1, T> operator ^ (const mat<3, 1, T>& a, const mat<3, 1, T>& b)
	{
		return { a(1) * b(2) - a(2) * b(1), a(2) * b(0) - a(0) * b(2), a(0) * b(1) - a(1) * b(0) };
	}


	// hadamard
	template<int Row,int Col,typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> operator&(const mat<Row, Col, T>& a, const mat<Row, Col, T>& b)
	{
		mat<Row, Col, T> ret;
		BinaryOp<T, Row* Col, Multiply>(&ret, &a, &b);
		return ret;
	}

	//kronecker
	template<int Row0,int Col0,int Row1,int Col1,typename T>
	static DEVICE_CALLABLE inline mat<Row0* Row1, Col0* Col1, T> operator%(const mat<Row0, Col0, T>& a, const mat<Row1, Col1, T>& b)
	{
		mat<Row0* Row1, Col0* Col1, T> ret;
		//BinaryOp<T, Row* Col, Multiply>(&ret, &a, &b);
		return ret;
	}


////////////////////////////////////rearrange//////////////////////////////
	template<int Row,int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, 1, T> get_column(const mat<Row, Col, T>& a, int column)
	{
		mat<Row, 1, T> ret;
		for (int i = 0; i < Row; i++)
		{
			ret(i) = a(i, column);
		}
		return ret;
	}

	template<int Row,int Col, typename T>
	static DEVICE_CALLABLE inline mat<Col, Row, T> transpose(const mat<Row, Col, T>& a)
	{
		mat<Col, Row, T> ret;
		for (int ri = 0; ri < Row; ri++)
		{
			for (int ci = 0; ci < Col; ci++)
			{
				ret(ci, ri) = a(ri, ci);
			}
		}
		return ret;
	}

	template<int N, typename T>
	static DEVICE_CALLABLE inline mat<N, 1, T> diagonal(const mat<N, N, T>& a)
	{
		mat<N, 1, T> ret;
		for (int i = 0; i < N; i++)
		{
			ret(i) = a(i, i);
		}
		return ret;
	}

	template<int N, typename T>
	static DEVICE_CALLABLE inline mat<N, N, T> diagonal(const mat<N, 1, T>& v)
	{
		mat<N, N, T> ret = get_uniform<N, N, T>(T(0));
		for (int i = 0; i < N; i++)
		{
			ret(i,i) = v(i);
		}
		return ret;
	}

	template<int Row,int Col, typename T>
	static DEVICE_CALLABLE inline const mat<Col* Row, 1, T>& vectorize(const mat<Row, Col, T>& a)
	{
		return *reinterpret_cast<const mat<Col* Row, 1, T>*>(&a);
	}



////////////////////////////////////matrix inverse //////////////////////////////
	template<int N, typename T>
	static	DEVICE_CALLABLE T determinant(const mat<N, N, T>& a) {
		if constexpr (N == 1)
		{
			return a;
		}
		else
		{
			T sum = 0;
			for (int i = 0; i < N; i++)
			{
				sum += a(0, i) * cofactor(a, i, 0);
			}
			return sum;
		}
	}

	template<int N, typename T>
	static DEVICE_CALLABLE inline mat<N - 1, N - 1, T> get_minor(const mat<N, N, T>& a, const int r, const int c)
	{
		mat<N - 1, N - 1, T> ret;
		for (int ri = 0; ri < N - 1; ri++)
		{
			for (int ci = 0; ci < N - 1; ci++)
			{
				int rr = ri < r ? ri : ri + 1;
				int cc = ci < c ? ci : ci + 1;
				ret(ci, ri) = a(cc, rr);
			}
		}
		return ret;
	}

	template<int N, typename T>
	constexpr DEVICE_CALLABLE inline T cofactor(const mat<N, N, T>& a, const int row, const int col)
	{
		T sign = (row + col) % 2 ? -1 : 1;
		return  determinant<N - 1, T>(get_minor(a, row, col)) * sign;
	}

	template<int N, typename T>
	DEVICE_CALLABLE inline mat<N, N, T> adjoint(const mat<N, N, T>& a) {
		mat<N, N, T> ret;
		for (int ri = 0; ri < N; ri++) {
			for (int ci = 0; ci < N; ci++)
			{
				ret(ri, ci) = cofactor(a, ri, ci);
			}
		}
		return ret;
	}

	template<int Row, int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> operator/(const mat<Row, Col, T>& m0, const mat<Row, Row, T>& m1) {

		mat<Row, Row, T> adj = adjoint(m1);
		T det = determinant<Row, T>(m1);
		auto inv = adj / det;
		return inv * m0;
	}


////////////////////////////////////norm //////////////////////////////
	template<int P >
	struct norm
	{
		template<int Row,int Col,typename T>
		static DEVICE_CALLABLE inline T apply(const mat<Row,Col,T>& a)
		{
			const auto& va = vectorize(a);
			T dot = transpose(va) * va;
			return sqrt(dot);
		}
	};

	template<int Row, int Col, typename T>
	static DEVICE_CALLABLE inline mat<Row, Col, T> normalize(const mat<Row, Col, T>& a)
	{
		return a / norm<2>::apply(a);
	}

	template<int P, int Row, int Col, typename T  >
	static DEVICE_CALLABLE  bool is_near(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1, T threshold = 0)
	{
		T diff_norm = norm<P>::apply(m0 - m1);
		T norm0 = norm<P>::apply(m0);
		T norm1 = norm<P>::apply(m1);
		T relative_diff = diff_norm / Max<T>::apply(norm0, norm1);
		return relative_diff <= threshold;
	}


	template< int Row, int Col, typename T  >
	static DEVICE_CALLABLE  bool operator==(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1)
	{
		for (int ri = 0; ri < Row ; ri++)
		{
			for (int ci = 0; ci < Col; ci++)
			{
				if (m0(ri, ci) != m1(ri, ci))
				{
					return false;
				}
			}
		}
		return true;
	}


}


////////////////////////////////////norm //////////////////////////////

template<int N,typename T>
using vec = matrix_math::mat<N, 1, T>;

using vec2f = vec<2,float>;
using vec3f = vec<3,float>;
using vec4f = vec<4,float>;

using vec2i = vec<2,int>;
using vec3i = vec<3,int>;
using vec4i = vec<4,int>;


using mat4x4f = matrix_math::mat<4, 4, float>;
using mat3x3f = matrix_math::mat<3, 3, float>;
using mat3x2f = matrix_math::mat<3, 2, float>;
using mat2x2f = matrix_math::mat<2, 2, float>;



