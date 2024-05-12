
module;

#include <assert.h>
#include <initializer_list>
#include <functional>


export module matrix_math: matrix;


#define mat_like(name) template<typename,int,int> typename name

namespace matrix_math
{
	template<typename T,int Row, int Col>
	struct matrix;

	template<typename T, int R, int C, mat_like(mat) >
	static const T& get_component(const mat<T, R, C>& m, int r, int c)
	{
		return m(r, c);
	}

	template<typename T, int R, int C, mat_like(mat) >
	static T& get_component(mat<T, R, C>& m, int r, int c)
	{
		return m(r, c);
	}

	template<typename T, int R,  mat_like(mat) >
	static const T& get_component(const mat<T, R, 1>& m, int r, int c)
	{
		return m(r);
	}

	template<typename T, int R,  mat_like(mat) >
	static T& get_component(mat<T, R, 1>& m, int r, int c)
	{
		return m(r);
	}

	template<typename T >
	static const T& get_component(const T& s, int r, int c)
	{
		return s;
	}

	template<typename T, int R, int C, typename Op_Out, typename Op_In, typename ...T_In >
	static void op_on_mat_component(Op_Out op_out, Op_In op_in, matrix<T, R, C>& out, const T_In& ...in)
	{
		for (int r = 0; r < R; r++)
		{
			for (int c = 0; c < C; c++)
			{
				op_out(get_component(out, r, c), op_in(get_component(in, r, c) ...));
			}
		}
	}

	template<typename T, int R, int C, typename Op_Out, typename Op_In, typename ...T_In >
	static matrix<T, R, C> op_on_mat_component(Op_Out op_out, Op_In op_in,  const T_In& ...in)
	{
		matrix<T, R, C> ret;
		op_on_mat_component(op_out, op_in, ret, in...);
		return ret;
	}

}

namespace matrix_math
{

	namespace op
	{
		template<typename T>
		struct identity
		{
			const T& operator()(const T& v)
			{
				return  v;
			}
		};

		template<typename T>
		struct assign
		{
			void operator()(T&out, const T& v)
			{
				out = v;
			}
		};

		template<typename T>
		struct plus_assign
		{
			void operator()(T&out, const T& v)
			{
				out += v;
			}
		};

		template<typename T>
		struct minus_assign
		{
			void operator()(T&out, const T& v)
			{
				out -= v;
			}
		};

		template<typename T>
		struct multiply_assign
		{
			void operator()(T&out, const T& v)
			{
				out *= v;
			}
		};

		template<typename T>
		struct divide_assign
		{
			void operator()(T&out, const T& v)
			{
				out /= v;
			}
		};

	}

}

namespace matrix_math
{

	template<typename T, int R, int C>
	struct matrix_view 
	{
		template<typename Get_Acc, int R_in, int C_in, mat_like(mat)  >
		void from(const mat<T, R_in, C_in>* m )
		{
			m_get = Get_Acc::get(m);
		}

		const T& operator()(int r, int c) const
		{
			return m_get(r, c);
		}

		operator matrix<T, R, C>()
		{
			return op_on_mat_component<T, R, C>(op::assign<T>{}, op::identity<T>{}, * this);
		}

		constexpr int size() const
		{
			return R * C;
		}

	private:
		std::function<const T& (int, int)> m_get;

	};

	template<typename T, int N>
	struct matrix_view <T,N,1>
	{
		template<typename Get_Acc, int R_in, int C_in, mat_like(mat)  >
		void from(const mat<T, R_in, C_in>* m )
		{
			m_get = Get_Acc::get(m);
		}

		const T& operator()(int i) const
		{
			return m_get(i);
		}

		operator matrix<T, N, 1>()
		{
			return op_on_mat_component<T, N, 1>(op::assign<T>{}, op::identity<T>{}, *this );
		}

		constexpr int size() const
		{
			return N;
		}

	private:
		std::function<const T& (int )> m_get;

	};

	///  acc getter
	struct transposer
	{
		template<typename T, int R, int C, mat_like(mat)>
		static auto get(const mat<T, R, C>* m)
		{
			return [m](int r, int c)
				{
					return get_component(*m, c, r);
				};
		}

	};

	struct vectorizer
	{
		template<typename T, int R, int C, mat_like(mat)>
		static auto get(const mat<T, R, C>* m)
		{
			return [m](int i)
				{
					int c = i / R;
					int r = i % R;
					return get_component(*m, r, c);
				};
		}


	};

}


export namespace matrix_math
{

	template<typename T,int Row, int Col>
	struct matrix
	{
		constexpr matrix() = default;

		constexpr matrix(const T& v)
		{
			for (int i = 0; i < Row * Col; i++)
			{
				m_data[i] = v;
			}
		}

		constexpr matrix(const std::initializer_list<T>& values)
		{
			assert(values.size() == Row * Col);
			for (int i = 0; const T& v  : values)
			{
				int r = i / Col;
				int c = i % Col;
				operator()(r, c) = v;
				i++;
			}
		}

		constexpr int size() const
		{
			return Row * Col;
		}

		constexpr T& operator() (int row, int col)
		{
			return m_data[row  + col * Row];
		}

		constexpr const T& operator() (int row, int col) const
		{
			return m_data[row + col * Row ];
		}

	private:
		T m_data[Row * Col];
	};

	template<typename T,int N >
	struct matrix<T,N,1>
	{
		constexpr matrix() = default;

		constexpr matrix(const T& v)
		{
			for (int i = 0; i < N; i++)
			{
				m_data[i] = v;
			}
		}

		constexpr matrix(const std::initializer_list<T>& values)
		{
			assert(values.size() == N);
			for (int i = 0; const T& v  : values)
			{
				operator()(i) = v;
				i++;
			}
		}

		constexpr int size() const
		{
			return N;
		}

		constexpr T& operator() (int i)
		{
			return m_data[i];
		}

		constexpr const T& operator() (int i) const
		{
			return m_data[i];
		}

	private:
		T m_data[N];
	};


	template<typename T,int R,int C, mat_like(mat)>
	matrix_view<T, R * C, 1 > vectorize(const mat<T, R, C>& m)
	{
		matrix_view<T, R * C, 1 > ret;
		ret.from<vectorizer>(&m);
		return ret;
	}

	template<typename T, int R, int C, mat_like(mat)>
	matrix_view<T, C, R> transpose(const mat<T, R, C>& m)
	{
		matrix_view<T, C, R> ret;
		ret.from<transposer>(&m);
		return ret;
	}


	//add m+m
	template<typename T,int R,int C, mat_like(mat)> matrix<T, R, C> operator+(const mat<T, R, C>& m0, const mat<T, R, C>& m1)
	{
		return op_on_mat_component<T,R,C>(op::assign<T>{},std::plus<T>{}, m0, m1);
	}

	//add m+s
	template<typename T,int R,int C, mat_like(mat)> matrix<T,R,C> operator+(const mat<T, R, C>& m, const T& s)
	{
		return op_on_mat_component<T,R,C>(op::assign<T>{},std::plus<T>{}, m, s);
	}

	//add s+m
	template<typename T,int R,int C, mat_like(mat)> matrix<T, R, C> operator+(const T& s, const mat<T, R, C>& m)
	{
		return op_on_mat_component<T, R, C>(op::assign<T>{},std::plus<T>{}, s, m);
	}

	//minus -m
	template<typename T, int R, int C, mat_like(mat)> matrix<T, R, C> operator-(const mat<T, R, C>& m)
	{
		return op_on_mat_component<T, R, C>(op::assign<T>{},std::negate<T>{}, m);
	}

	//minus m-m
	template<typename T,int R,int C, mat_like(mat)> matrix<T,R,C> operator-(const mat<T, R, C>& m0, const mat<T, R, C>& m1)
	{
		return op_on_mat_component<T, R, C>(op::assign<T>{},std::minus<T>{}, m0, m1);
	}

	//minus m-s
	template<typename T,int R,int C, mat_like(mat)> matrix<T,R,C> operator-(const mat<T, R, C>& m, const T& s)
	{
		return op_on_mat_component<T, R, C>(op::assign<T>{},std::minus<T>{}, m, s);
	}

	//divides m/s
	template<typename T, int R, int C, mat_like(mat)> matrix<T, R, C> operator/(const mat<T, R, C>& m, const T& s)
	{
		return op_on_mat_component<T, R, C>(op::assign<T>{},std::divides<T>{}, m, s);
	}

	//multiplies m*s
	template<typename T,int R,int C,mat_like(mat)> matrix<T, R, C> operator*(const mat<T, R, C>& m, const T& s)
	{
		return op_on_mat_component<T, R, C>(op::assign<T>{},std::multiplies<T>{}, m, s);
	}

	//multiplies s*m
	template<typename T, int R, int C, mat_like(mat)> matrix<T, R, C> operator*(const T& s, const mat<T, R, C>& m)
	{
		return op_on_mat_component<T, R, C>(op::assign<T>{},std::multiplies<T>{}, s, m);
	}

	//multiplies m*m (includes m*v and v*m)
	template<typename T, int R, int N, int C, mat_like(mat0), mat_like(mat1) >
	matrix<T, R, C> operator*(const  mat0<T,R,N>& m0, const mat1<T, N, C>& m1 )
	{
		matrix<T, R, C> ret{};
		for (int r = 0; r < R; r++)
		{
			for (int c = 0; c < C; c++)
			{
				for (int n = 0; n < N; n++)
				{
					get_component(ret, r, c) += get_component(m0, r, n) * get_component(m1, n, c);
				}
			}
		}
		return ret;
	}


	//m*=s
	template<typename T,int R,int C,mat_like(mat)> mat<T, R, C>& operator*=(mat<T, R, C>& m, const T& s)
	{
		op_on_mat_component<T, R, C>(op::multiply_assign<T>{},op::identity<T>{}, m, s);
		return m;
	}

	//m+=s
	template<typename T,int R,int C,mat_like(mat)> mat<T, R, C>& operator+=(mat<T, R, C>& m, const T& s)
	{
		op_on_mat_component<T, R, C>(op::plus_assign<T>{},op::identity<T>{}, m, s);
		return m;
	}

	//m-=s
	template<typename T,int R,int C,mat_like(mat)> mat<T, R, C>& operator-=(mat<T, R, C>& m, const T& s)
	{
		op_on_mat_component<T, R, C>(op::minus_assign<T>{},op::identity<T>{}, m, s);
		return m;
	}

	//m/=s
	template<typename T,int R,int C,mat_like(mat)> mat<T, R, C>& operator/=(mat<T, R, C>& m, const T& s)
	{
		op_on_mat_component<T, R, C>(op::divide_assign<T>{},op::identity<T>{}, m, s);
		return m;
	}

}

