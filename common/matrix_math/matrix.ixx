
module;

#include <assert.h>
#include <initializer_list>
#include <functional>
#include <array>


export module matrix_math: matrix;


#define mat_like(name) template<typename,int,int> typename name

template<typename T>
struct testDebugT;

namespace matrix_math
{
	template<typename T,int Row, int Col>
	struct matrix;

	struct accessor
	{
		template<typename M >
		static decltype(auto) apply(M&& m, int r, int c)
		{
			if constexpr (requires (M m) { m(r, c); })
			{
				return std::forward<M>(m)(r, c);
			}
			else
			{
				return std::forward<M>(m);
			}
		}
	};


	template<typename T, int R, int C, typename Op_Out, typename Op_In, mat_like(mat), typename ...T_In >
	static void op_on_mat_component(Op_Out op_out, Op_In op_in, mat<T, R, C>& out, const T_In& ...in)
	{
		for (int r = 0; r < R; r++)
		{
			for (int c = 0; c < C; c++)
			{
				op_out(accessor::apply(out, r, c), op_in(accessor::apply(in, r, c) ...));
			}
		}
	}

	template<typename T, int R, int C,  typename Op_Out, typename Op_In, typename ...T_In >
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
		template<typename T> struct identity { const T& operator()(const T& v) { return  v; } };

		template<typename T> struct assign { void operator()(T&out, const T& v) { out = v; } };

		template<typename T> struct plus_assign { void operator()(T&out, const T& v) { out += v; } };

		template<typename T> struct minus_assign { void operator()(T&out, const T& v) { out -= v; } };

		template<typename T> struct multiply_assign { void operator()(T&out, const T& v) { out *= v; } };

		template<typename T> struct divide_assign { void operator()(T&out, const T& v) { out /= v; } };

	}

}

namespace matrix_math
{
	namespace view
	{

		template<typename U, typename T, int R, int C>
		struct imp
		{

			template<typename Get_Acc, int R_in, int C_in, typename M, typename ...P >
			void from(M&& m, P&& ...p)
			{
				Get_Acc index_map{ std::forward<P>(p)... };
				m_get = [&m, index_map](int r, int c) -> U
					{ 
						auto [ri, ci] = index_map.operator()<R , C, R_in, C_in> (r, c);
						return accessor::apply(std::forward<M>(m), ri, ci);
					};
			}

			constexpr const T& operator()(int r, int c) const
			{
				return m_get(r, c);
			}

			constexpr T& operator()(int r, int c) 
			{
				return m_get(r, c);
			}

			operator matrix<T, R, C>()
			{
				return op_on_mat_component<T, R, C >(op::assign<T>{}, op::identity<T>{}, * this);
			}

			constexpr int size() const
			{
				return R * C;
			}

		private:
			std::function<U (int, int)> m_get;

		};

		template<typename T, int R, int C>
		struct read :public imp<const T&, T, R, C> { };

		template<typename T, int N >
		struct read<T, N, 1> :public imp<const T&, T, N, 1>
		{
			constexpr const T& operator()(int i) const
			{
				return internal::operator()(i, 0);
			}

			friend accessor;
		private:
			using internal = imp<const T&, T, N, 1>;
			using internal::operator();
		};

		template<typename T, int N >
		struct read<T, 1, N> :public imp<const T&, T, 1, N>
		{
			constexpr const T& operator()(int i) const
			{
				return internal::operator()(0, i);
			}

			friend accessor;
		private:
			using internal = imp<const T&, T, 1, N>;
			using internal::operator();
		};

		template<typename T, int R, int C>
		struct write :public imp<T&, T, R, C> 
		{
			template<mat_like(mat)>
			write& operator=(const mat<T, R, C>& m)
			{
				op_on_mat_component<T, R, C >(op::assign<T>{}, op::identity<T>{}, * this, m);
				return *this;
			}
		};

		template<typename T, int N >
		struct write<T, N, 1> :public imp<T&, T, N, 1>
		{
		private:
			using internal = imp<T&, T, N, 1>;

		public:
			constexpr T& operator()(int i) 
			{
				return internal::operator()(i, 0);
			}

			template<mat_like(mat)>
			write& operator=(const mat<T, N, 1>& m)
			{
				op_on_mat_component<T, N, 1 >(op::assign<T>{}, op::identity<T>{}, * this, m);
				return *this;
			}

			using  internal::operator=;

			friend accessor;
		private:
			using internal::operator();
		};

		template<typename T, int N >
		struct write<T, 1, N> :public imp<T&, T, 1, N>
		{
			constexpr T& operator()(int i) 
			{
				return internal::operator()(0, i);
			}

			template<mat_like(mat)>
			write& operator=(const mat<T, 1, N>& m)
			{
				op_on_mat_component<T, 1, N >(op::assign<T>{}, op::identity<T>{}, * this, m);
				return *this;
			}

			friend accessor;
		private:
			using internal = imp<T&, T, 1, N>;
			using internal::operator();
		};



		struct row_col
		{
			int row;
			int col;
		};

		struct transposer
		{
			template<int R_out, int C_out, int R_in, int C_in >
			row_col operator()(int r, int c) const
			{
				static_assert(R_out == C_in);
				static_assert(C_out == R_in );

				return { c,r };
			}

		};

		struct vectorizer
		{
			template<int R_out, int C_out, int R_in, int C_in >
			row_col operator()(int r_out, int c_out) const
			{
				static_assert(C_out == 1);
				static_assert(R_out == R_in * C_in);

				int c_in = r_out / R_in;
				int r_in = r_out % R_in;

				return { r_in, c_in };
			}
		};

		struct map_column
		{
			template<int R_out, int C_out, int R_in, int C_in >
			row_col operator()(int r_out, int c_out) const
			{
				static_assert(C_out == 1);
				static_assert(R_out == R_in );

				return { r_out, col };
			}

			int col;

		};
	}

}


namespace matrix_math
{
	namespace internal
	{
		template<typename T, int R, int C>
		struct mat
		{
			constexpr mat() = default;

			constexpr mat(const std::initializer_list<T>& values)
			{
				assert(values.size() == R * C);
				for (int i = 0; const T & v : values)
				{
					int r = i / C;
					int c = i % C;
					operator()(r, c) = v;
					i++;
				}
			}

			constexpr int size() const
			{
				return R * C;
			}

			constexpr T& operator() (int row, int col)
			{
				return m_data[row + col * R];
			}

			constexpr const T& operator() (int row, int col) const
			{
				return m_data[row + col * R];
			}

		private:
			T m_data[R*C];

		};
	}
}


export namespace matrix_math
{

	template<typename T,int Row, int Col>
	struct matrix : public internal::mat<T, Row, Col>
	{
		constexpr matrix() = default;
		constexpr matrix(const std::initializer_list<T>& values) : internal::mat<T, Row, Col>(values) { }
	};

	template<typename T,int N >
	struct matrix<T,N,1> : public internal::mat<T, N, 1>
	{
		constexpr matrix() = default;
		constexpr matrix(const std::initializer_list<T>& values) : mat(values) { }

		constexpr T& operator() (int i)
		{
			return mat::operator()(i, 0);
		}

		constexpr const T& operator() (int i) const
		{
			return mat::operator()(i, 0);
		}

		friend accessor;
	private:
		using mat = internal::mat<T, N, 1>;
		using mat::operator();
	};

	template<typename T,int N >
	struct matrix<T,1,N> : public internal::mat<T, 1, N>
	{
		constexpr matrix() = default;
		constexpr matrix(const std::initializer_list<T>& values) : mat(values) { }

		constexpr T& operator() (int i)
		{
			return mat::operator()(i, 0);
		}

		constexpr const T& operator() (int i) const
		{
			return mat::operator()(i, 0);
		}

		friend accessor;
	private:
		using mat = internal::mat<T, 1, N>;
		using mat::operator();
	};


	template<typename T,int R,int C, mat_like(mat)>
	view::read<T, R * C, 1 > vectorize(const mat<T, R, C>& m)
	{
		view::read<T, R * C, 1 > ret;
		ret.from<view::vectorizer, R, C>(m);
		return ret;
	}

	template<typename T,int R,int C, mat_like(mat)>
	view::write<T, R * C, 1 > vectorize(mat<T, R, C>& m)
	{
		view::write<T, R * C, 1 > ret;
		ret.from<view::vectorizer, R, C>(m);
		return ret;
	}

	template<typename T,int R,int C, mat_like(mat)>
	view::read<T, R, 1 > column(const mat<T, R, C>& m, int col)
	{

		testDebugT<mat<T, R, C>>();

		view::read<T, R , 1 > ret;
		ret.from<view::map_column, R, C>(m, col);
		return ret;
	}

	template<typename T,int R,int C, mat_like(mat)>
	view::write<T, R, 1 > column(mat<T, R, C>& m, int col)
	{

		view::write<T, R , 1 > ret;
		ret.from<view::map_column, R, C>(m, col);
		return ret;
	}

	template<typename T, int R, int C, mat_like(mat)>
	view::read<T, C, R> transpose(const mat<T, R, C>& m)
	{
		view::read<T, C, R> ret;
		ret.from<view::transposer, R, C>(m);
		return ret;
	}

	template<typename T, int R, int C, mat_like(mat)>
	view::write<T, C, R> transpose(mat<T, R, C>& m)
	{
		view::write<T, C, R> ret;
		ret.from<view::transposer, R, C>(m);
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
					accessor::apply(ret, r, c) += accessor::apply(m0, r, n) * accessor::apply(m1, n, c);
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

