

#include <initializer_list>
#include <numeric>
#include <algorithm>

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

	vec<Row, T>& operator[](int c) { return col_vecs[c]; }
	const vec<Row, T>& operator[](int c) const { return col_vecs[c]; }

	T * get_flat(){ return flat; }
	T const* get_flat()const { return flat; }



public:
	static constexpr size_t row = Row;
	static constexpr size_t col = Col;
private:
	union {
		T data[row][col];
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
	T& operator[](int i) { return data[i]; }
	const T& operator[](int i) const { return data[i]; }

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

template<size_t row, size_t col, typename T>
static inline mat<row, col, T> get_identity()
{
	mat<row, col, T> ret;
	for (int ri = 0; ri < row; ri++)
	{
		for (int ci = 0; ci < col; ci++)
		{
			if (ri == ci)
			{
				ret[ci][ri] = 1.f;
			}
			else
			{
				ret[ci][ri] = 0.f;
			}

		}
	}
	return ret;
}

////////////////////////////


///////////////////////

template<typename T>
struct Add
{
	T operator()(T v0, T v1)
	{
		return v0 + v1;
	}
};

template<typename T>
struct Substract
{
	T operator()(T v0, T v1)
	{
		return v0 - v1;
	}
};

template<typename T>
struct Multiply
{
	T operator()(T v0, T v1)
	{
		return v0 * v1;
	}
};

template<typename T>
struct Divide
{
	T operator()(T v0, T v1)
	{
		return v0 / v1;
	}
};


template<size_t Row, size_t Col, typename T, typename F>
static mat<Row, Col, T> matrix_elementwise_binary_op(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1,F fn)
{
	mat<Row, Col, T> ret;
	std::transform(m0.get_flat(), m0.get_flat() + Row * Col, m1.get_flat(), ret.get_flat(), fn);
	return ret;
}

template<size_t Row, size_t Col, typename T, typename F>
static mat<Row, Col, T> matrix_elementwise_unary_op(const mat<Row, Col, T>& m0, F fn)
{
	mat<Row, Col, T> ret;
	std::transform(m0.get_flat(), m0.get_flat() + Row * Col, ret.get_flat(), fn);
	return ret;
}


///////////////////////
#define MATRIX_ELEMENTWISE_BINARY_OP(x,op) \
template<size_t Row,size_t Col,typename T> \
static mat<Row, Col, T> operator##x(const mat<Row, Col, T>& m0,const mat<Row, Col, T>& m1) \
{ \
	return matrix_elementwise_binary_op<Row,Col,T>(m0, m1,op<T>()); \
} \

//a0+a1
MATRIX_ELEMENTWISE_BINARY_OP(+,Add)

//a0-a1
MATRIX_ELEMENTWISE_BINARY_OP(-,Substract)

//a0*a1
MATRIX_ELEMENTWISE_BINARY_OP(*,Multiply)

//a0/a1
MATRIX_ELEMENTWISE_BINARY_OP(/,Divide)
///////////////////////



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


template<int P, size_t Row, size_t Col, typename T  >
static bool is_near(const mat<Row, Col, T>& m0, const mat<Row, Col, T>& m1, T threshold = 0)
{
	T norm = Vectorized_Norm<P>()(m0 - m1);
	return norm <= threshold;
}
/////////////////////




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
using mat2x2f = mat<2, 2, float>;
