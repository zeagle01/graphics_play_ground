

#include <initializer_list>

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



private:
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
private:
	T data[Row];
};






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
