
#pragma once

#include <initializer_list>
#include <numeric>
#include <algorithm>
#include <limits>
#include <functional>

template<size_t Row,size_t Col,typename T>
class mat;

template<size_t Row,typename T>
using vec = mat<Row, 1, T>;

template<size_t Row,size_t Col,typename T>
class mat 
{
public:
	mat(){}
	mat(const std::initializer_list<std::initializer_list<T>> data_list)//col major
	{
		int ci = 0;
		for ( auto it_col = data_list.begin(); it_col != data_list.end(); it_col++,ci++)
		{
			int ri = 0;
			for ( auto it_row = it_col->begin(); it_row != it_col->end(); it_row++,ri++)
			{
				data[ci][ri]=*it_row;
			}
		}

	}

	mat(const std::initializer_list<vec<Row, T>> data_list)//col major
	{
		int ci = 0;
		for ( auto it_col = data_list.begin(); it_col != data_list.end(); it_col++,ci++)
		{
			for (int ri = 0; ri < Row;  ri++)
			{
				data[ci][ri] = (*it_col)(ri);
			}
		}

	}

	vec<Row, T>& column(int c) { return col_vecs[c]; }
	const vec<Row, T>& column(int c) const { return col_vecs[c]; }

	T& operator()(int ri,int ci) { 
		return data[ci][ri]; 
	}
	const T& operator()(int ri,int ci) const { return data[ci][ri]; }

	T * get_flat(){ return flat; }
	T const* get_flat()const { return flat; }



public:
	static constexpr size_t row = Row;
	static constexpr size_t col = Col;
private:
	union {
		T data[col][row];
		T flat[row * col];
		vec<Row, T> col_vecs[col];
	};
};

template<size_t Row,typename T>
class mat<Row, 1, T> 
{
public:
	mat() {}
	mat(const std::initializer_list<T> data_list)//col major
	{
		int ri = 0;
		for ( auto it_row = data_list.begin(); it_row != data_list.end(); it_row++,ri++)
		{
			data[ri] = *it_row;
		}

	}

	operator T()
	{
		return data[0];
	}

	vec<Row, T>& column(int c) { return *this; }
	const vec<Row, T>& column(int c) const { return *this; }

	T& operator()(int ri) { return data[ri]; }
	const T& operator()(int ri) const { return data[ri]; }

	T& operator()(int ri, int ci) { return data[ri]; }
	const T& operator()(int ri, int ci) const { return data[ri]; }

	T * get_flat(){ return data; }
	T const* get_flat()const { return data; }
private:
	T data[Row];
};




////////////////////////////
template<size_t Row,size_t Col,typename T>
static mat<Row, Col, T> get_uniform(T value)
{
	mat<Row, Col, T> ret;
	for (int i = 0; i < Row * Col; i++) 
	{
		ret.get_flat()[i] = value; 
	}
	return ret;
}

template<size_t N, typename T>
static inline mat<N, N, T> get_identity()
{
	mat<N, N, T> ret;
	for (int ri = 0; ri < N; ri++)
	{
		for (int ci = 0; ci < N; ci++)
		{
			if (ri == ci)
			{
				ret(ri,ci) = 1.f;
			}
			else
			{
				ret(ri,ci) = 0.f;
			}

		}
	}
	return ret;
}

template<size_t N, typename T>
static inline mat<N - 1, N - 1, T> get_minor(const mat<N, N, T>& a, const int r, const int c)
{
	mat<N - 1, N - 1, T> ret;
	for (int ri = 0; ri < N - 1; ri++)
	{
		for (int ci = 0; ci < N - 1; ci++)
		{
			int rr = ri < r ? ri : ri + 1;
			int cc = ci < c ? ci : ci + 1;
			ret(ci,ri) = a(cc,rr);
		}
	}
	return ret;
}

template<size_t Row,size_t Col, typename T>
static inline mat<Col, Row, T> transpose(const mat<Row, Col, T>& m)
{
	mat<Col, Row, T> ret;
	for (int ri = 0; ri < Row; ri++)
	{
		for (int ci = 0; ci < Col; ci++)
		{
			ret(ci,ri) = m(ri,ci);
		}
	}
	return ret;
}

////////////////////////////


///////////////////////

template<typename T>
struct Add { T operator()(T v0, T v1) { return v0 + v1; } };
template<typename T>
struct Self_Add { T operator()(T v0, T v1) { return v0 += v1; } };

template<typename T>
struct Substract { T operator()(T v0, T v1) { return v0 - v1; } };
template<typename T>
struct Self_Substract { T operator()(T v0, T v1) { return v0 -= v1; } };

template<typename T>
struct Multiply { T operator()(T v0, T v1) { return v0 * v1; } };
template<typename T>
struct Self_Multiply { T operator()(T v0, T v1) { return v0 *= v1; } };

template<typename T>
struct Divide { T operator()(T v0, T v1) { return v0 / v1; } };
template<typename T>
struct Self_Divide { T operator()(T v0, T v1) { return v0 /= v1; } };


template<size_t Row, size_t Col, typename T, typename F>
static mat<Row, Col, T> matrix_elementwise_binary_op(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1,F fn)
{
	mat<Row, Col, T> ret;
	std::transform(m0.get_flat(), m0.get_flat() + Row * Col, m1.get_flat(), ret.get_flat(), fn);
	return ret;
}

template<size_t Row, size_t Col, typename T, typename F>
static void matrix_elementwise_self_binary_op(mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1, F fn)
{
	std::transform(m0.get_flat(), m0.get_flat() + Row * Col, m1.get_flat(), m0.get_flat(), fn);
}

template<size_t Row, size_t Col, typename T, typename F>
static mat<Row, Col, T> matrix_elementwise_unary_op(const mat<Row, Col, T>& m0, F fn)
{
	mat<Row, Col, T> ret;
	std::transform(m0.get_flat(), m0.get_flat() + Row * Col, ret.get_flat(), fn);
	return ret;
}

template<size_t Row, size_t Col, typename T, typename F>
static void matrix_elementwise_self_unary_op(mat<Row, Col, T>& m0, F fn)
{
	std::transform(m0.get_flat(), m0.get_flat() + Row * Col, m0.get_flat(), fn);
}


///////////////////////
#define MATRIX_ELEMENTWISE_BINARY_OP(x,op) \
template<size_t Row,size_t Col,typename T> \
static mat<Row, Col, T> operator##x(const mat<Row, Col, T>& m0,const mat<Row, Col, T>& m1) \
{ \
	return matrix_elementwise_binary_op<Row,Col,T>(m0, m1,op<T>()); \
} \

#define MATRIX_ELEMENTWISE_SELF_BINARY_OP(x,op) \
template<size_t Row,size_t Col,typename T> \
static void operator##x(mat<Row, Col, T>& m0,const mat<Row, Col, T>& m1) \
{ \
	matrix_elementwise_self_binary_op<Row,Col,T>(m0, m1,op<T>()); \
} \

//a0+a1
MATRIX_ELEMENTWISE_BINARY_OP(+,Add)

MATRIX_ELEMENTWISE_SELF_BINARY_OP(+=,Self_Add)

//a0-a1
MATRIX_ELEMENTWISE_BINARY_OP(-,Substract)

//a0*a1
MATRIX_ELEMENTWISE_BINARY_OP(*,Multiply)

//a0/a1
MATRIX_ELEMENTWISE_BINARY_OP(/,Divide)


#define MATRIX_SCALAR_OP(x) \
template<size_t Row,size_t Col,typename T>  \
static mat<Row, Col, T> operator##x(const mat<Row, Col, T>& m,T s) \
{ \
	auto fn=[s](T v) \
	{\
		return v x s;\
	};\
	return matrix_elementwise_unary_op<Row, Col, T>(m, fn);\
}\
\
template<size_t Row,size_t Col,typename T>  \
static mat<Row, Col, T> operator##x(T s,const mat<Row, Col, T>& m) \
{ \
	auto fn=[s](T v) \
	{\
		return s x v;\
	};\
	return matrix_elementwise_unary_op<Row, Col, T>(m, fn);\
}\

#define MATRIX_SELF_SCALAR_OP(x) \
template<size_t Row,size_t Col,typename T>  \
static  void operator##x(mat<Row, Col, T>& m,T s) \
{ \
	auto fn=[s](T v) \
	{\
		return v x s;\
	};\
	return matrix_elementwise_self_unary_op<Row, Col, T>(m, fn);\
}\


//// a op s  and s op a
MATRIX_SCALAR_OP(+)
MATRIX_SCALAR_OP(-)
MATRIX_SCALAR_OP(*)
MATRIX_SCALAR_OP(/)
MATRIX_SELF_SCALAR_OP(+=)
MATRIX_SELF_SCALAR_OP(-=)
MATRIX_SELF_SCALAR_OP(*=)
MATRIX_SELF_SCALAR_OP(/=)

// -a
template<size_t Row,size_t Col,typename T>  
static mat<Row, Col, T> operator-(const mat<Row, Col, T>& m) 
{ 
	auto fn=[](T v) 
	{
		return -v ;
	};
	return matrix_elementwise_unary_op<Row, Col, T>(m, fn);
}

///////////////////////
// a0.a1 (matrix multiply matrix)
template<size_t Row, size_t Col, size_t N,typename T>
static inline mat<Row, Col, T> operator&(const mat<Row, N, T>& m0, const mat<N, Col, T>& m1)
{
	mat<Row, Col, T> ret = get_uniform<Row, Col, T>(T(0));
	for (int ci = 0; ci < Col; ci++)
	{
		for (int ni = 0; ni < N; ni++)
		{
			ret.column(ci) = ret.column(ci) + m0.column(ni) * m1(ni, ci);
		}
	}
	return ret;
}

// dot product
template<size_t Row, size_t Col, typename T>
static inline vec<Row, T> operator&( const vec<Row, T>& v,const mat<Row, Col, T>& m)
{
	return operator&(m, v);
}


//cross product
template< typename T>
static inline vec<3, T> cross(const vec<3, T>& a, const vec<3, T>& b)
{
	return { a(1) * b(2) - a(2) * b(1), a(2) * b(0) - a(0) * b(2), a(0) * b(1) - a(1) * b(0) };
}


//outer product
template< typename T, int  N>
static inline mat<N, N, T> operator^(const vec<N, T>& a, const vec<N, T>& b)
{
	mat<N, N, T> ret;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			ret(i, j) = a(i) * b(j);
		}
	}
	return ret;
}




/////////////////////
template<int P >
struct Vectorized_Norm
{

	template< size_t Row, size_t Col, typename T>
	T operator()(const mat<Row, Col, T>& m)
	{
		T p = P;
		auto power_fn = [p](T v)
		{
			return std::pow<T>(v, p);
		};

		auto powered = matrix_elementwise_unary_op<Row, Col, T >(m, power_fn);
		T ret = std::accumulate(powered.get_flat(), powered.get_flat() + Row * Col, T(0));
		ret = std::pow(ret, 1 / p);
		return ret;
	}
};

//infinity norm
template<>
struct Vectorized_Norm<0>
{
	template< size_t Row, size_t Col, typename T>
	T operator()(const mat<Row, Col, T>& m)
	{
		auto max_value = [](T accumulated, T new_v)
		{
			T abs_new_v = std::abs(new_v);
			T abs_accumulated = std::abs(accumulated);
			return abs_new_v > abs_accumulated ? abs_new_v : abs_accumulated;
		};

		T ret = std::accumulate(m.get_flat(), m.get_flat() + Row * Col, T(0), max_value);
		return ret;
	}
};

//normalized vector
template< size_t Row, typename T  >
static vec<Row, T> normalize(const vec<Row, T>& v)
{
	vec<Row, T> ret = v;
	T l = Vectorized_Norm<2>()(v);
	if (l >= std::numeric_limits<T>::min())
	{
		ret = v / Vectorized_Norm<2>()(v);
	}
	return ret;
}



template<int P, size_t Row, size_t Col, typename T  >
static bool is_near(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1, T threshold = 0)
{
	T norm = Vectorized_Norm<P>()(m0 - m1);
	T norm0 = Vectorized_Norm<P>()(m0);
	T relative_diff = norm / norm0;
	return relative_diff <= threshold;
}

template<int P, size_t Row, size_t Col, typename T  >
struct is_near1
{
	T threshold = T(0);
	bool operator()(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1)
	{
		T norm = Vectorized_Norm<P>()(m0 - m1);
		T norm0 = Vectorized_Norm<P>()(m0);
		T relative_diff = norm / norm0;
		return relative_diff <= threshold;
	}
};

template< size_t Row, size_t Col, typename T  >
static bool operator==(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1)
{
	return is_near<0>(m0, m1);
}




/////////////////////
template<size_t N, typename T>
struct determinant {

	static	T get(const mat<N, N, T>& a) {
		T sum = 0;
		for (size_t i = 0; i < N; i++)
		{
			sum += a(0,i) * cofactor(a, i, 0);
		}
		return sum;
	}
};

template<typename T>
struct determinant<1, T>
{
	static T get(const mat<1, 1, T>& a)
	{
		return a(0,0);
	}
};


template<size_t N, typename T>
constexpr inline T cofactor(const mat<N, N, T>& a, const int row, const int col)
{
	T sign = (row + col) % 2 ? -1 : 1;
	return  determinant<N - 1, T>::get(get_minor(a, row, col)) * sign;
}

template<size_t N, typename T>
inline mat<N, N, T> adjoint(const mat<N, N, T>& a) {
	mat<N, N, T> ret;
	for (size_t ri = 0; ri < N; ri++) {
		for (size_t ci = 0; ci < N; ci++)
		{
			ret(ri,ci) = cofactor(a, ri, ci);
		}
	}
	return ret;
}

template<size_t Row, size_t Col, typename T>
static inline mat<Row, Col, T> operator|(const mat<Row, Col, T>& m0, const mat<Row, Row, T>& m1) {

	mat<Row, Row, T> adj = adjoint(m1);
	T det = determinant<Row, T>::get(m1);
	auto inv = adj / det;
	return inv & m0;
}

/////////////////////




/////////////list of matrix////////////

template<int P, size_t Row, size_t Col, typename T  >
static bool is_near_list(const mat<Row, Col, T>* m0, const mat<Row, Col, T>* m1, int list_size, T threshold = 0)
{
	return std::inner_product(m0, m0 + list_size, m1, true, std::logical_and<>(), is_near1<P, Row, Col, T>(threshold));
}

//template< size_t Row, size_t Col, typename T  >
//static bool operator==(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1)
//{
//	return is_near<0>(m0, m1);
//}




//////////////////////type alias///////////////////////
using vec2f = vec<2, float>;
using vec3f = vec<3, float>;
using vec4f = vec<4, float>;

using vec2i = vec<2, int>;
using vec3i = vec<3, int>;
using vec4i = vec<4, int>;

template<typename T>
using mat4x4 = mat<4, 4, T>;

using mat4x4f = mat<4, 4, float>;
using mat3x3f = mat<3, 3, float>;
using mat3x2f = mat<3, 2, float>;
using mat2x2f = mat<2, 2, float>;




namespace matrix_math
{

#define MAT_DATA(Row,Col,T) \
	T data[Row * Col]; \
	const T& operator()(int ri, int ci) const { return data[ri + ci * Row]; }\
	T& operator()(int ri, int ci) { return data[ri + ci * Row]; } \
	const T* operator&() const {return data;} \
	T* operator&(){ return data;} \

#define VEC_ACCESS(T) \
		const T& operator()(int i) const { return data[i]; } \
		T& operator()(int i) { return data[i]; }  \


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


	template<typename T> struct Add { static T apply(T v0, T v1) { return v0 + v1; } };
	template<typename T> struct Substract { static T apply(T v0, T v1) { return v0 - v1; } };
	template<typename T> struct Multiply { static T apply(T v0, T v1) { return v0 * v1; } };
	template<typename T> struct Divide { static T apply(T v0, T v1) { return v0 / v1; } };


	template<typename T, int N, template<typename> typename Op>
	static inline void BinaryOp(T* a, const T* b, const T* c)
	{
		for (int i = 0; i < N; i++)
		{
			a[i] = Op<T>::apply(b[i], c[i]);
		}
	}

#define COMPONENTWISE_BINARY(name,component_op) \
	template<int Row,int Col,typename T> \
	static inline mat<Row, Col, T> name(mat<Row, Col, T>& a, mat<Row, Col, T>& b)\
	{\
		mat<Row, Col, T> ret;\
		BinaryOp<T, Row* Col, component_op>(&ret, &a, &b);\
		return ret;\
	}\

COMPONENTWISE_BINARY(operator+,Add)
COMPONENTWISE_BINARY(operator-,Substract)

	template<int Row, int N, int Col, typename T>
	static inline mat<Row, Col, T> operator*(const mat<Row, N, T>& a, const mat<N, Col, T>& b)
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

	template<int Row,int Col, typename T>
	static inline mat<Row, Col, T> operator*(const mat<Row, Col, T>& a, const T& s)
	{
		mat<Row, Col, T> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			(&ret)[i] = (&a)[i] * s;
		}
		return ret;
	}

	template<int Row,int Col, typename T>
	static inline mat<Row, Col, T> operator*(const T& s, const mat<Row, Col, T>& a) { return a * s; }

	template<int Row,int Col, typename T>
	static inline mat<Row, Col, T> operator/(const mat<Row, Col, T>& a, const T& s)
	{
		mat<Row, Col, T> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			(&ret)[i] = (&a)[i] / s;
		}
		return ret;
	}


	//cross product
	template< typename T>
	static inline mat<3, 1, T> operator ^ (const mat<3, 1, T>& a, const mat<3, 1, T>& b)
	{
		return { a(1) * b(2) - a(2) * b(1), a(2) * b(0) - a(0) * b(2), a(0) * b(1) - a(1) * b(0) };
	}

	template<int Row,int Col, typename T>
	static inline mat<Row, 1, T> get_column(const mat<Row, Col, T>& a, int column)
	{
		mat<Row, 1, T> ret;
		for (int i = 0; i < Row; i++)
		{
			ret(i) = a(i, column);
		}
		return ret;
	}

	template<int Row,int Col, typename T>
	static inline mat<Col, Row, T> transpose(const mat<Row, Col, T>& a)
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


	//inverse wise

	template<int N, typename T>
	static	T determinant(const mat<N, N, T>& a) {
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
	static inline mat<N - 1, N - 1, T> get_minor(const mat<N, N, T>& a, const int r, const int c)
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
	constexpr inline T cofactor(const mat<N, N, T>& a, const int row, const int col)
	{
		T sign = (row + col) % 2 ? -1 : 1;
		return  determinant<N - 1, T>(get_minor(a, row, col)) * sign;
	}

	template<int N, typename T>
	inline mat<N, N, T> adjoint(const mat<N, N, T>& a) {
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
	static inline mat<Row, Col, T> operator/(const mat<Row, Col, T>& m0, const mat<Row, Row, T>& m1) {

		mat<Row, Row, T> adj = adjoint(m1);
		T det = determinant<Row, T>(m1);
		auto inv = adj / det;
		return inv * m0;
	}




	//////////////////
	template<size_t N, typename T>
	static inline mat<N, N, T> get_identity()
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

	template<size_t Row, size_t Col, typename T>
	static inline mat<Row, Col, T> get_uniform(T value)
	{
		mat<Row, Col, T> ret;
		for (int i = 0; i < Row * Col; i++)
		{
			(&ret)[i] = value;
		}
		return ret;
	}


using vec4 = mat<4, 1, float>;
using vec3 = mat<3, 1, float>;
using vec2 = mat<2, 1, float>;

using rvec4 = mat<1, 4, float>;
using rvec3 = mat<1, 3, float>;
using rvec2 = mat<1, 2, float>;

using mat4 = mat<4, 4, float>;
using mat3 = mat<3, 3, float>;
using mat2 = mat<2, 2, float>;

}




