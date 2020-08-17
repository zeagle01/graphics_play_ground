
#pragma once

#include <initializer_list> 
#include <functional>
#include <map>


namespace soft_render
{


#define VEC_BASIC()\
		mat() {} \
		mat(const mat& other) { *this = other; } \
		mat(const std::initializer_list<T>& il) { int i = 0; for (auto it = il.begin(); it != il.end(); it++) { assert(i < size); data[i] = *it; i++; } } \
		mat& operator=(const mat& other) { for (int i = 0; i < size; i++) { data[i] = other[i]; } return *this; } \
		T& operator[](size_t i) { return data[i]; } \
		const T& operator[](size_t i) const { return data[i]; } \
		T* as_vec() { return data; } \
		const T* as_vec() const { return data; } \



//#define MAT_BASIC()\
//		mat() {} \
//		mat(const mat& other) { *this = other; } \
//		mat(const std::initializer_list<T>& il) { int i = 0; for (auto it = il.begin(); it != il.end(); it++) { assert(i < size); data[i] = *it; i++; } } \
//		mat& operator=(const mat& other) { for (int i = 0; i < size; i++) { data[i] = other[i]; } return *this; } \
//		T& operator[](size_t i) { return data[i]; } \
//		const T& operator[](size_t i) const { return data[i]; } \


	template<size_t N>
	struct Sizable
	{
		static constexpr size_t size = N;
	};

	template<size_t N,size_t M>
	struct Mat_Size :Sizable<N * M>
	{
		static constexpr size_t row = N;
		static constexpr size_t col = M;
	};

	////////////////////////////matrix
	template<size_t row,size_t col,typename T>
	class mat;

	template<size_t N,typename T>
	using vec = mat<N, 1, T>;

	template<size_t row,size_t col,typename T>
	class mat :public Mat_Size<row, col>
	{
	public:
		mat() {} 
		mat(const mat& other) { *this = other; } 
		mat(const std::initializer_list<std::initializer_list<T>>& il)
		{
			int ci = 0;
			for (auto it_col = il.begin(); it_col != il.end(); it_col++)
			{ 
				int ri = 0;
				for (auto it_row = it_col->begin(); it_row != it_col->end(); it_row++)
				{
					vecs[ci][ri] = *it_row;
					assert(ri < row);  ri++;
				}
				assert(ci < col);  ci++; 
			} 
		} 

		mat(const std::initializer_list<vec<row, T>>& il)
		{
			int ci = 0;
			for (auto it_col = il.begin(); it_col != il.end(); it_col++)
			{ 
				vecs[ci] = *it_col;
				assert(ci < col);  ci++;
			} 
		} 
		mat& operator=(const mat& other) {
			for (int i = 0; i < size; i++)
			{ 
				vecs->as_vec()[i] = other.as_vec()[i]; 
			}
			return *this;
		} 
		vec<row, T>& operator[](size_t ci) { return vecs[ci]; }
		const vec<row, T>& operator[](size_t ci) const { return vecs[ci]; } 

		vec<col, T> get_row(size_t ri)const 
		{
			vec<col, T> ret;
			for (int ci = 0; ci < col; ci++) {
				ret[ci] = vecs[ci][ri];
			}
			return ret;
		}

		vec<row, T> get_col(size_t ci)const
		{
			return vecs[ci];
		}

		T* as_vec() { return data; };
		const T* as_vec() const { return data; } 



	private:
		union
		{
			vec<row, T> vecs[col];
			T data[row * col];
		};
	};

	//this for mat<1,1> is the same with vec<1>
	template<typename T>
	class mat<1, 1, T> :public Mat_Size<1, 1>
	{
	public:
		mat() {}
		mat(const mat& other) { *this = other; }
		mat(const std::initializer_list<std::initializer_list<T>>& il)
		{
			int ci = 0;
			for (auto it_col = il.begin(); it_col != il.end(); it_col++)
			{
				int ri = 0;
				for (auto it_row = it_col->begin(); it_row != it_col->end(); it_row++)
				{
					data = *it_row;
					assert(ri < row);  ri++;
				}
				assert(ci < col);  ci++;
			}
		}

		mat& operator=(const mat& other) {
			data = other.data;
			return *this;
		}
		T* operator[](size_t ci) { assert(ci == 0); return &data; }
		const T* operator[](size_t ci) const { assert(ci == 0); return &data; }

		vec<col, T> get_row(size_t ri)const
		{
			return {data};
		}

		vec<row, T> get_col(size_t ci)const
		{
			return {data};
		}

		T* as_vec() { return *data; };
		const T* as_vec() const { return *data; }



	public:
		T data;
	};


	///vector
	template<size_t N,typename T>
	class mat<N, 1, T> :public Sizable<N>
	{
		T data[size]; 
	public:
		VEC_BASIC();


	};

	template<typename T>
	class mat<2,1, T>:public Sizable<2>
	{
	public:
		union {
			struct { T x, y; };
			struct { T u, v; };
			T data[2];
		};
		VEC_BASIC()

	};

	template<typename T>
	class mat<3, 1, T> :public Sizable<3>
	{
	public:
		union {
			struct { T x, y, z; };
			T data[3];
		};
		VEC_BASIC()

	};

	template<typename T>
	class mat<4, 1, T> :public Sizable<4>
	{
	public:
		union {
			struct { T x,y,z,w; };
			T data[4];
		};
		VEC_BASIC()

	};


	//////////////matrix and vector function//////////////////
	//uniform_fill
	template<size_t N,size_t M, typename T>
	static inline mat<N, M, T> uniform_fill(T value) 
	{ 
		mat<N, M, T> ret;
		for (int i = 0; i < N * M; i++) { ret.as_vec()[i] = value; }
		return ret; 
	}
	template<size_t N, typename T>
	static inline vec<N, T> uniform_fill(T value) { return uniform_fill<N, 1, T>(value); }

	//a+b
	template<size_t N,size_t M,typename T>
	static inline mat<N, M, T> operator+(const mat<N, M, T>& a, const mat<N, M, T>& b)
	{
		mat<N,M,T> ret;
		for (int i = 0; i < N * M; i++)
		{
			ret.as_vec()[i] = a.as_vec()[i] + b.as_vec()[i];
		}
		return ret;
	}
	template<size_t N, typename T>
	static inline vec<N, T> operator+(const vec<N, T>& a, const vec<N, T>& b) { return operator+<N, 1, T>(a, b); }

	//a-b
	template<size_t N,size_t M,typename T>
	static inline mat<N, M, T> operator-(const mat<N, M, T>& a, const mat<N, M, T>& b)
	{
		mat<N,M,T> ret;
		for (int i = 0; i < N * M; i++)
		{
			ret.as_vec()[i] = a.as_vec()[i] - b.as_vec()[i];
		}
		return ret;
	}
	template<size_t N, typename T>
	static inline vec<N, T> operator-(const vec<N, T>& a, const vec<N, T>& b) { return operator-<N, 1, T>(a, b); }

	//a==b
	template<size_t N,size_t M,typename T>
	static inline bool operator==(const mat<N, M, T>& a, const mat<N, M, T>& b)
	{
		for (int i = 0; i < N * M; i++)
		{
			if (a.as_vec()[i] != b.as_vec()[i])
			{
				return false;
			}
		}
		return true;
	}
	template<size_t N, typename T>
	static inline bool operator==(const vec<N, T>& a, const vec<N, T>& b) { return operator==<N, 1, T>(a, b); }

	//a*b
	template<size_t N,size_t M,typename T>
	static inline mat<N, M, T> operator*(const mat<N, M, T>& a, const mat<N, M, T>& b)
	{
		vec<N,M,T> ret;
		for (int i = 0; i < N * M; i++)
		{
			ret.as_vec()[i] = a.as_vec()[i] * b.as_vec()[i];
		}
		return ret;
	}
	template<size_t N,typename T>
	static inline vec<N, T> operator*(const vec<N, T>& a, const vec<N, T>& b) { return operator*<N,1,T>(a, b); }

	//s*a
	template<size_t N,size_t M,typename T>
	static inline mat<N,M,T> operator*(T a, const mat<N,M, T>& b)
	{
		vec<N,T> ret;
		for (int i = 0; i < N*M; i++)
		{
			ret.as_vec()[i] = a * b.as_vec()[i];
		}
		return ret;
	}
	template<size_t N,typename T>
	static inline vec<N, T> operator*(T a, const vec<N, T>& b) { return operator*<N, 1, T>(a, b); }

	//a/s
	template<size_t N,size_t M,typename T>
	static inline mat<N,M,T> operator/( const mat<N,M, T>& a,T s)
	{
		mat<N,M,T> ret;
		for (int i = 0; i < N*M; i++)
		{
			ret.as_vec()[i] = a.as_vec()[i] / s;
		}
		return ret;
	}
	template<size_t N,typename T>
	static inline vec<N, T> operator/(const vec<N, T>& a, T s) { return operator/<N, 1, T>(a, s); }


	//component wise dot(even for matrix)
	template<size_t N, size_t M, typename T>
	static inline T dot(const mat<N, M, T>& a, const mat<N, M, T>& b)
	{
		T ret=0;
		for (int i = 0; i < N*M; i++)
		{
			ret += a.as_vec()[i] * b.as_vec()[i];
		}
		return ret;
	}
	template<size_t N,  typename T>
	static inline T dot(const vec<N, T>& a, const vec<N, T>& b) { return dot<N, 1, T>(a, b); }

	//type conversion
	template<size_t N, size_t M, typename src_T,typename dst_T>
	static inline mat<N, M, dst_T> type_conversion(const mat<N, M, src_T>& a)
	{
		mat<N, M, dst_T> ret;
		for (int i = 0; i < N*M; i++)
		{
			ret.as_vec()[i] = dst_T(a.as_vec()[i]);
		}
		return ret;
	}
	template<size_t N, typename src_T, typename dst_T>
	static inline vec<N, dst_T> type_conversion(const vec<N, src_T>& a) { return type_conversion<N, 1, src_T, dst_T>(a); }


	///////////////////////////////matrix only///////////////////////////////////
	template<size_t row,size_t col,typename T>
	static inline mat<row,col,T> get_identity()
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

	template<size_t N, typename T>
	static inline mat<N - 1, N - 1, T> get_minor(const mat<N, N, T>& a, const int r, const int c)
	{
		mat<N - 1, N - 1, T> ret;
		for (int ri = 0; ri < N-1; ri++)
		{
			for (int ci = 0; ci < N-1; ci++)
			{
				int rr = ri < r ? ri : ri + 1;
				int cc = ci < c ? ci : ci + 1;
				ret[ci][ri] = a[cc][rr];
			}
		}
		return ret;
	}







	////////////////////////////math////////////////////////////////////////////

	//for static recursion 
	template<size_t N, typename T>
	struct determinant {

		static	T get(const mat<N, N, T>& a) {
			T sum = 0;
			for (size_t i = 0; i < N; i++)
			{
				sum += a[0][i] * cofactor(a, i, 0);
			}
			return sum;
		}
	};

	template<typename T>
	struct determinant<1, T>
	{
		static T get(const mat<1, 1, T>& a)
		{
			return a[0][0];
		}
	};

	template<size_t N,typename T>
	constexpr inline T cofactor(const mat<N,N,T>& a,const int row,const int col)
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
				ret[ci][ri] = cofactor(a, ri, ci);
			}
		}
		return ret;
	}

	template<size_t N, typename T>
	inline mat<N, N, T> invert(const mat<N, N, T>& a) {

		mat<N, N, T> adj = adjoint(a);

		T det = dot(a.get_row(0), adj.get_col(0));

		assert(det != 0.f);

		return adj/det;
	}

	template<size_t n,typename T>
	static inline vec<n, T> matrix_multiply(mat<n, n, T>& A,vec<n,  T>& b)
	{
		vec<n, T> ret;
		for (int ri = 0; ri < n; ri++)
		{
			ret[ri] = dot(A.get_row(ri), b);
		}
		return ret;
	}

	template<size_t n,typename T>
	static inline mat<n, n, T> matrix_multiply(mat<n, n, T>& A, mat<n, n, T>& B)
	{
		mat<n, n, T> ret;
		for (int ri = 0; ri < n; ri++)
		{
			for (int ci = 0; ci < n; ci++)
			{
				ret[ci][ri] = dot(A.get_row(ri), B[ci]);
			}
		}
		return ret;
	}

	template<size_t n,typename T>
	static inline vec<n, T> solve(mat<n, n, T>& A, vec<n, T>& b)
	{
		mat<n, n, T> inv = invert(A);
		return matrix_multiply(inv, b);
	}


	//////////////////////type alias///////////////////////
	using vec2f = vec<2, float>;
	using vec3f = vec<3, float>;
	using vec4f = vec<4, float>;

	using vec2i = vec<2, int>;
	using vec3i = vec<3, int>;
	using vec4i = vec<4, int>;

	using mat3x3f = mat<3, 3, float>;
	using mat2x2f = mat<2, 2, float>;








}
