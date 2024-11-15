
module;

#include <assert.h>
#include <initializer_list>
#include <functional>
#include <array>
#include <type_traits>
#include <numeric>

export module matrix_math: matrix;

namespace matrix_math
{

	namespace matrix_imp
	{
		template<typename T, int R, int C>
		struct basic_info
		{
			static constexpr int row_num = R;
			static constexpr int col_num = C;
			using type = T;
		};

		struct accessor
		{
			template<typename M >
			static constexpr decltype(auto) apply(M&& m, int r, int c)
			{
				if constexpr (requires { m(0, 0); })
				{
					return std::forward<M>(m)(r, c);
				}
				else
				{
					return std::forward<M>(m);
				}
			}
		};

		template<typename M>
		concept matrix_like = requires (M  m)
		{
			std::decay_t<M>::row_num;
			std::decay_t<M>::col_num;
			matrix_imp::accessor::apply(m, 0, 0);
		};

		struct assigner
		{
			template<matrix_like m0_t, matrix_like m1_t>
			static constexpr void apply(m0_t&& m0, m1_t&& m1)
			{
				constexpr int R = std::decay_t<m0_t>::row_num;
				constexpr int C = std::decay_t<m0_t>::col_num;
				constexpr int R1 = std::decay_t<m1_t>::row_num;
				constexpr int C1 = std::decay_t<m1_t>::col_num;
				static_assert(R == R1);
				static_assert(C == C1);
				using T = std::decay_t<m0_t>::type;
				using T1 = std::decay_t<m1_t>::type;

				static_assert(std::is_same_v<T, T1>);

				for (int r = 0; r < R; r++)
				{
					for (int c = 0; c < C; c++)
					{
						accessor::apply(std::forward<m0_t>(m0), r, c) = accessor::apply(std::forward<m1_t>(m1), r, c);
					}
				}
			}
		};
	}


	namespace matrix_imp
	{
		template<typename T, int R, int C, typename d0_t, typename d1_t, typename op>
		struct expression : public basic_info< T, R, C>
		{
			constexpr expression(const d0_t& d0, const d1_t& d1, const op& _op) : m_d0(d0), m_d1(d1), m_op(_op) {}

			constexpr decltype(auto) operator()(int r, int c) const
			{
				return m_op(accessor::apply(m_d0, r, c), accessor::apply(m_d1, r, c));
			}

		public:
			const d0_t& m_d0;
			const d1_t& m_d1;
			op m_op;
		};

		template<typename T, int R, int C, typename d0_t, typename op>
		struct unitary_expression : public basic_info< T, R, C>
		{
			constexpr unitary_expression(const d0_t& d0, const op& _op) : m_d0(d0), m_op(_op) {}

			constexpr decltype(auto) operator()(int r, int c) const
			{
				return m_op(accessor::apply(m_d0, r, c));
			}

		public:
			const d0_t& m_d0;
			op m_op;
		};

		template<typename T, int R, int C, typename d0_t, typename d1_t, typename op>
		struct free_expression : public basic_info< T, R, C>
		{
			constexpr free_expression(const d0_t& d0, const d1_t& d1, const op& _op) : m_d0(d0), m_d1(d1), m_op(_op) {}

			constexpr decltype(auto) operator()(int r, int c) const
			{
				return m_op(m_d0, m_d1, r, c);
			}

		public:
			const d0_t& m_d0;
			const d1_t& m_d1;
			op m_op;

		};

		template<typename T, typename d0_t, typename d1_t, typename op>
		struct free_expression<T, 1, 1, d0_t, d1_t, op> : public basic_info< T, 1, 1>
		{
			constexpr free_expression(const d0_t& d0, const d1_t& d1, const op& _op) : m_d0(d0), m_d1(d1), m_op(_op) {}
			constexpr decltype(auto) operator()(int r, int c) const { return m_op(m_d0, m_d1, r, c); }
			constexpr  operator T() { return operator()(0, 0); }

		public:
			const d0_t& m_d0;
			const d1_t& m_d1;
			op m_op;

		};


		template<template<typename> typename op, typename d0_t, typename d1_t >
		auto build_expression(const d0_t& d0, const d1_t& d1)
		{

			if constexpr (matrix_like<d0_t> && matrix_like<d1_t>)
			{
				constexpr int R = std::decay_t<d0_t>::row_num;
				constexpr int C = std::decay_t<d0_t>::col_num;
				constexpr int R1 = std::decay_t<d1_t>::row_num;
				constexpr int C1 = std::decay_t<d1_t>::col_num;

				static_assert(R == R1);
				static_assert(C == C1);

				using T = std::decay_t<d0_t>::type;
				using T1 = std::decay_t<d1_t>::type;

				static_assert(std::is_same_v<T, T1>);

				return matrix_imp::expression<T, R, C, d0_t, d1_t, op<T>>{ d0, d1, op<T>{} };
			}
			else if constexpr (matrix_like<d0_t>)
			{
				constexpr int R = std::decay_t<d0_t>::row_num;
				constexpr int C = std::decay_t<d0_t>::col_num;
				using T = std::decay_t<d0_t>::type;
				return matrix_imp::expression<T, R, C, d0_t, d1_t, op<T>>{ d0, d1, op<T>{} };
			}
			else if constexpr (matrix_like<d1_t>)
			{
				constexpr int R = std::decay_t<d1_t>::row_num;
				constexpr int C = std::decay_t<d1_t>::col_num;
				using T = std::decay_t<d1_t>::type;
				return matrix_imp::expression<T, R, C, d0_t, d1_t, op<T>>{ d0, d1, op<T>{} };
			}
			else
			{
				constexpr int R = 1;
				constexpr int C = 1;
				using T = d0_t;
				return matrix_imp::expression<T, R, C, d0_t, d1_t, op<T>>{ d0, d1, op<T>{} };
			}

		}

		template<template<typename> typename op, typename d0_t >
		auto build_expression(const d0_t& d0)
		{
			constexpr int R = std::decay_t<d0_t>::row_num;
			constexpr int C = std::decay_t<d0_t>::col_num;
			using T = std::decay_t<d0_t>::type;
			return matrix_imp::unitary_expression<T, R, C, d0_t, op<T>>{ d0, op<T>{} };
		}

		struct mat_x_mat
		{
			template<typename m0_t, typename m1_t>
			decltype(auto) operator()(const m0_t& m0, const m1_t& m1, int r, int c) const
			{
				constexpr int N = std::decay_t<m0_t>::col_num;
				using T = std::decay_t<m0_t>::type;

				T ret{};
				for (int i = 0; i < N; i++)
				{
					ret += accessor::apply(m0, r, i) * accessor::apply(m1, i, c);
				}
				return ret;
			}
		};

	}

	template<typename T, int R, int C> struct matrix;
	template<int Row_size, int Col_size, typename M > decltype(auto) submatrix(M&& m, int row_offset, int col_offset);
	template<typename M> decltype(auto) column(M&& m, int col);

	namespace matrix_imp
	{

		struct LU_solver
		{
		private:
			template< matrix_imp::matrix_like m_t >
			static void swap_row(m_t& A, int i, int j, int* permutation)
			{
				constexpr int C = std::decay_t<m_t>::col_num;

				for (int c = 0; c < C; c++)
				{
					std::swap(A(i, c), A(j, c));
				}

				std::swap(permutation[i], permutation[j]);
			}

			template< typename vec >
			static int find_max_index(const vec& v, int n)
			{
				auto max_value = std::abs(v(0));
				int ret = 0;

				for (int i = 1; i < n; i++)
				{
					auto curr_value = std::abs(v(i));
					if (curr_value > max_value)
					{
						max_value = curr_value;
						ret = i;
					}
				}
				return ret;
			}


			template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
			static void LU_decompose(m0_t& LU, int* permutation, m1_t& A)
			{

				constexpr int R1 = std::decay_t<m1_t>::row_num;
				constexpr int C1 = std::decay_t<m1_t>::col_num;
				using T = std::decay_t<m0_t>::type;
				static_assert(R1 == C1); //square
				constexpr int N = R1;

				if constexpr (N == 1)
				{
					LU = A;
				}
				else
				{

					auto A_first_col = matrix_math::column(A, 0);

					int max_I = find_max_index(A_first_col, N);

					swap_row(A, 0, max_I, permutation);

					LU(0, 0) = A(0, 0);

					auto L10 = matrix_math::submatrix<N - 1, 1>(LU, 1, 0);
					auto U01 = matrix_math::submatrix<1, N - 1>(LU, 0, 1);

					auto A10 = matrix_math::submatrix<N - 1, 1>(A, 1, 0);
					auto A01 = matrix_math::submatrix<1, N - 1>(A, 0, 1);

					U01 = A01;
					L10 = A10 / A(0, 0);

					matrix<T, N - 1, N - 1>	 S = matrix_math::submatrix<N - 1, N - 1>(A, 1, 1) - L10 * U01;
					auto LU11 = matrix_math::submatrix<N - 1, N - 1>(LU, 1, 1);

					LU_decompose(LU11, permutation + 1, S);

				}

			}

			template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
			static auto LU_solve(const m0_t& _B, const m1_t& LU, const std::vector<int>& permutation)
			{

				constexpr int R = std::decay_t<m0_t>::row_num;
				constexpr int C = std::decay_t<m0_t>::col_num;
				constexpr int R1 = std::decay_t<m1_t>::row_num;
				constexpr int C1 = std::decay_t<m1_t>::col_num;

				static_assert(R == R1);
				static_assert(R1 == C1); //square

				using T = std::decay_t<m0_t>::type;
				using T1 = std::decay_t<m1_t>::type;
				static_assert(std::is_same_v<T, T1>);

				constexpr int N = R1;

				matrix<T, R, C> B;

				for (int ri = 0; ri < R; ri++)
				{
					for (int ci = 0; ci < C; ci++)
					{
						matrix_imp::accessor::apply(B, ri, ci) = matrix_imp::accessor::apply(_B, permutation[ri], ci);
					}
				}


				matrix<T, N, C> Y;

				//L(r,i) Y(i,c) = B(r,c)
				for (int c = 0; c < C; c++)
				{
					for (int r = 0; r < N; r++)
					{
						T s = matrix_imp::accessor::apply(B, r, c);
						for (int i = 0; i < r; i++)
						{
							s -= LU(r, i) * matrix_imp::accessor::apply(Y, i, c);
						}
						matrix_imp::accessor::apply(Y, r, c) = s;
					}

				}

				matrix<T, N, C> X;

				//U(r,i) X(i,c) = Y(r,c)
				for (int c = 0; c < C; c++)
				{
					for (int r = N - 1; r >= 0; r--)
					{
						T s = matrix_imp::accessor::apply(Y, r, c);
						for (int i = r + 1; i < N; i++)
						{
							s -= LU(r, i) * matrix_imp::accessor::apply(X, i, c);
						}
						matrix_imp::accessor::apply(X, r, c) = s / LU(r, r);
					}
				}

				return X;
			}

		public:
			template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
			static auto LU_inverse(const m0_t& B, const m1_t& A)
			{

				constexpr int R = std::decay_t<m0_t>::row_num;
				constexpr int C = std::decay_t<m0_t>::col_num;
				constexpr int R1 = std::decay_t<m1_t>::row_num;
				constexpr int C1 = std::decay_t<m1_t>::col_num;

				static_assert(R == R1);
				static_assert(R1 == C1); //square

				using T = std::decay_t<m0_t>::type;
				using T1 = std::decay_t<m1_t>::type;
				static_assert(std::is_same_v<T, T1>);

				constexpr int N = R1;
				matrix<T, N, N> LU{};

				std::vector<int> permutation(N);
				std::iota(permutation.begin(), permutation.end(), 0);

				matrix<T, N, N> copy_A = A;

				LU_decompose(LU, permutation.data(), copy_A);

				return LU_solve(B, LU, permutation);
			}

		};

	}


	namespace matrix_imp
	{
		template<typename T, int R, int C>
		struct buffer :public basic_info< T, R, C>
		{
			constexpr buffer() = default;
			template<int values_num> constexpr buffer(const T(&values)[values_num])
			{
				static_assert(values_num == R * C);
				for (int i = 0; const T & v : values)
				{
					int r = i / C;
					int c = i % C;
					operator()(r, c) = v;
					i++;
				}
			}

			constexpr T& operator()(int r, int c) { return m_data[r + c * R]; } 
			constexpr const T& operator()(int r, int c) const { return m_data[r + c * R]; }

			template<matrix_like m_t >
			constexpr buffer(const m_t& m)
			{
				assigner::apply(*this, m);
			}

			template<matrix_like m_t >
			constexpr buffer& operator=(m_t&& m)
			{
				assigner::apply(*this, std::forward<m_t>(m));
				return *this;
			}

		private:
			std::array<T, R* C> m_data;
		};
	}

	namespace matrix_imp
	{
		struct row_col
		{
			int row, col;
		};

		template<typename M, typename index_mapper>
		struct view : public basic_info< typename std::decay_t<M>::type, std::decay_t<index_mapper>::R_out, std::decay_t<index_mapper>::C_out >
		{
			view(M&& mat, index_mapper im) : m_mat(std::forward<M>(mat)), m_index_mapper(std::forward<index_mapper>(im)) {}

			constexpr decltype(auto) operator()(int r, int c) const
			{
				auto [row, col] = m_index_mapper(r, c);
				return accessor::apply(m_mat, row, col);
			}

			constexpr decltype(auto) operator()(int r, int c)
			{
				auto [row, col] = m_index_mapper(r, c);
				return accessor::apply(m_mat, row, col);
			}

			template<matrix_like m_t >
			constexpr view& operator=(m_t&& m)
			{
				assigner::apply(*this, std::forward<m_t>(m));
				return *this;
			}

		private:
			M m_mat;
			index_mapper m_index_mapper;
		};

		template<typename M, typename index_mapper>
		struct col_view : public view<M, index_mapper>
		{
		private:
			using imp = view<M, index_mapper>;
		public:
			col_view(M&& mat, index_mapper im) : imp(std::forward<M>(mat), im) {}

			constexpr decltype(auto) operator()(int i) const { return imp::operator()(i, 0); }
			constexpr decltype(auto) operator()(int i) { return imp::operator()(i, 0); }

			using imp::operator=;
			friend struct accessor;
		private:
			using imp::operator();
		};

		template<typename M, typename index_mapper>
		struct row_view : public view<M, index_mapper>
		{
		private:
			using imp = view<M, index_mapper>;
		public:
			row_view(M&& mat, index_mapper im) : imp(std::forward<M>(mat), im) {}

			constexpr decltype(auto) operator()(int i) const { return imp::operator()(0, i); }
			constexpr decltype(auto) operator()(int i) { return imp::operator()(0, i); }

			using imp::operator=;
			friend struct accessor;
		private:
			using imp::operator();
		};


		template<typename ...Q>
		struct view_builder
		{
			template<template<int, int, typename ...> typename index_mapper_t, typename M, typename ...P >
			static decltype(auto) apply(M&& m, P&& ...p)
			{
				constexpr int R = std::decay_t<M>::row_num;
				constexpr int C = std::decay_t<M>::col_num;
				using im_t = index_mapper_t<R, C, Q...>;
				constexpr int R_out = im_t::R_out;
				constexpr int C_out = im_t::C_out;

				im_t im{ std::forward<P>(p)... };

				if constexpr (C_out == 1)
				{
					return matrix_imp::col_view<M, im_t >(std::forward<M>(m), im);
				}
				else if constexpr (R_out == 1)
				{
					return matrix_imp::row_view<M, im_t >(std::forward<M>(m), im);
				}
				else
				{
					return matrix_imp::view<M, im_t >(std::forward<M>(m), im);
				}
			}

		};
	}


	namespace matrix_imp
	{
		namespace index_mapper
		{
			template<int R_in, int C_in>
			struct transposer
			{
				static constexpr int R_out = C_in;
				static constexpr int C_out = R_in;

				row_col operator()(int r, int c) const
				{
					return { c,r };
				}
			};

			template<int R_in, int C_in>
			struct vectorizer
			{
				static constexpr int R_out = R_in * C_in;
				static constexpr int C_out = 1;

				row_col operator()(int r_out, int c_out) const
				{
					int c_in = r_out / R_in;
					int r_in = r_out % R_in;

					return { r_in, c_in };
				}
			};

			template<int R_in, int C_in>
			struct get_column
			{
				static constexpr int R_out = R_in;
				static constexpr int C_out = 1;

				row_col operator()(int r_out, int c_out) const
				{
					return { r_out, col };
				}

				int col;

			};

			template<int R_in, int C_in>
			struct get_cofactor
			{
				static constexpr int R_out = R_in - 1;
				static constexpr int C_out = C_in - 1;

				row_col operator()(int r_out, int c_out) const
				{
					int c_in = c_out < col ? c_out : c_out + 1;
					int r_in = r_out < row ? r_out : r_out + 1;

					return { r_in, c_in };
				}

				int row;
				int col;
			};

			template<int R_in, int C_in, typename Rsize_t, typename Csize_t>
			struct get_submatrix
			{
				static constexpr int R_out = Rsize_t::value;
				static constexpr int C_out = Csize_t::value;

				row_col operator()(int r_out, int c_out) const
				{
					int c_in = c_out + col_offset;
					int r_in = r_out + row_offset;

					return { r_in, c_in };
				}

				int row_offset;
				int col_offset;
			};

		}
	}
	namespace matrix_imp
	{
		template<typename list>
		struct get_first_row_col_imp;

		template<matrix_imp::matrix_like m0_t, matrix_imp::matrix_like ...mi_t>
		struct get_first_row_col_imp<std::tuple<m0_t, mi_t...>>
		{
			static constexpr int row_num = std::decay_t<m0_t>::row_num;
			static constexpr int col_num = std::decay_t<m0_t>::col_num;
			using type = std::decay_t<m0_t>::type;
		};

		template< matrix_imp::matrix_like ... m_t>
		using get_first_row_col = get_first_row_col_imp<std::tuple<m_t...>>;

		struct assign_clumns
		{
			template<matrix_like m0_t, matrix_like m1_t>
			static constexpr void apply(m0_t&& m0, m1_t&& m1, int& c_offset)
			{
				constexpr int R0 = std::decay_t<m0_t>::row_num;

				constexpr int R1 = std::decay_t<m1_t>::row_num;
				constexpr int C1 = std::decay_t<m1_t>::col_num;

				static_assert(R0 == R1);

				for (int r = 0; r < R1; r++)
				{
					for (int c = 0; c < C1; c++)
					{
						accessor::apply(m0, r, c_offset + c) = accessor::apply(m1, r, c);
					}
				}
				c_offset += C1;
			}
		};
		
	}

}


export
namespace matrix_math
{
	template<typename T, int R, int C>
	struct matrix : public matrix_imp::buffer<T, R, C> 
	{ 

	private:
		using buffer = matrix_imp::buffer<T, R, C>;
	public:
		constexpr matrix() = default;
		template<int values_num> constexpr matrix(const T(&values)[values_num]): buffer(values){}
		template<matrix_imp::matrix_like m_t > matrix(const m_t& exp) : buffer(exp) {}
	};

	template<typename T, int N>
	struct matrix<T, N, 1> : public matrix_imp::buffer<T, N, 1>
	{
	private:
		using buffer = matrix_imp::buffer<T, N, 1>;
	public:
		constexpr matrix() = default;
		template<int values_num> constexpr matrix(const T(&values)[values_num]): buffer(values){}
		template<matrix_imp::matrix_like m_t > constexpr matrix(const m_t& exp) : buffer(exp) {}

		constexpr T& operator()(int i) { return buffer::operator()(i, 0); }
		constexpr const T& operator()(int i) const { return buffer::operator()(i, 0); }

		friend struct matrix_imp::accessor;
	private:
		using buffer::operator();
	};

	template<typename T, int N>
	struct matrix<T, 1, N> : public matrix_imp::buffer<T, 1, N>
	{
	private:
		using buffer = matrix_imp::buffer<T, 1, N>;
	public:
		constexpr matrix() = default;
		template<int values_num> constexpr matrix(const T(&values)[values_num]) : buffer(values) {}
		template<matrix_imp::matrix_like m_t > constexpr matrix(const m_t& exp) : buffer(exp) {}

		constexpr T& operator()(int i) { return buffer::operator()(0, i); }
		constexpr const T& operator()(int i) const { return buffer::operator()(0, i); }

		friend struct matrix_imp::accessor;
	private:
		using buffer::operator();
	};

	template<typename T >
	struct matrix<T, 1, 1> : public matrix_imp::buffer<T, 1, 1>
	{
	private:
		using buffer = matrix_imp::buffer<T, 1, 1>;
	public:
		constexpr matrix() = default;
		template<matrix_imp::matrix_like m_t > constexpr matrix(const m_t& exp) : buffer(exp) {}

		constexpr T& operator()() { return buffer::operator()(0, 0); }
		constexpr const T& operator()() const { return buffer::operator()(0, 0); }

		friend struct matrix_imp::accessor;
	private:
		using buffer::operator();
	};


	// constructors
	template<matrix_imp::matrix_like ... m_t>
	constexpr auto from_columns(m_t&& ...sub_mats)
	{
		using T = matrix_imp::get_first_row_col<m_t...>::type;
		constexpr int R = matrix_imp::get_first_row_col<m_t...>::row_num;
		constexpr int total_column_num = (0 + ... + std::decay_t<m_t>::col_num);

		matrix<T, R, total_column_num> ret;
		int c_offset = 0;
		(matrix_imp::assign_clumns::apply(ret, sub_mats, c_offset), ...);
		return ret;
	}

	//get views
	template<typename M>
	decltype(auto) transpose(M&& m)
	{
		return matrix_imp::view_builder<>::apply<matrix_imp::index_mapper::transposer>(std::forward<M>(m));
	}

	template<typename M>
	decltype(auto) vectorize(M&& m)
	{
		return matrix_imp::view_builder<>::apply<matrix_imp::index_mapper::vectorizer>(std::forward<M>(m));
	}

	template<typename M>
	decltype(auto) column(M&& m, int col)
	{
		return matrix_imp::view_builder<>::apply<matrix_imp::index_mapper::get_column>(std::forward<M>(m), col);
	}

	template<typename M>
	decltype(auto) cofactor(M&& m, int row, int col)
	{
		return matrix_imp::view_builder<>::apply<matrix_imp::index_mapper::get_cofactor>(std::forward<M>(m), row, col);
	}

	template<int Row_size, int Col_size, typename M >
	decltype(auto) submatrix(M&& m, int row_offset, int col_offset)
	{
		using Row_size_t = std::integral_constant<int, Row_size>;
		using Col_size_t = std::integral_constant<int, Col_size>;
		return matrix_imp::view_builder<Row_size_t, Col_size_t>::apply<matrix_imp::index_mapper::get_submatrix>(std::forward<M>(m), row_offset, col_offset);
	}

	///////////////operators 
	// m+m
	template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
	auto operator+(const m0_t& m0 , const m1_t& m1)
	{
		return build_expression<std::plus>(m0, m1);
	}

	// m+s
	template< matrix_imp::matrix_like m0_t, typename T >
	auto operator+(const m0_t& m0, const T& m1)
	{
		return build_expression<std::plus>(m0, m1);
	}

	// s+m
	template<  typename T, matrix_imp::matrix_like m0_t>
	auto operator+(const T& m0, const m0_t& m1)
	{
		return build_expression<std::plus>(m0, m1);
	}

	// -m
	template< matrix_imp::matrix_like m0_t  >
	auto operator-(const m0_t& m0)
	{
		return build_expression<std::negate>(m0);
	}

	//minus m-m
	template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
	auto operator-(const m0_t& m0, const m1_t& m1)
	{
		return build_expression<std::minus>(m0, m1);
	}

	//minus m-s
	template< matrix_imp::matrix_like m0_t, typename m1_t>
	auto operator-(const m0_t& m0, const m1_t& m1)
	{
		return build_expression<std::minus>(m0, m1);
	}

	//divides m/s
	template< matrix_imp::matrix_like m0_t, typename m1_t>
	auto operator/(const m0_t& m0, const m1_t& m1)
	{
		return build_expression<std::divides>(m0, m1);
	}

	//multiplies m*s
	template< matrix_imp::matrix_like m0_t, typename m1_t>
	auto operator*(const m0_t& m0, const m1_t& m1)
	{
		return build_expression<std::multiplies>(m0, m1);
	}

	//multiplies m*s
	template<typename m0_t, matrix_imp::matrix_like m1_t >
	auto operator*(const m0_t& m0, const m1_t& m1)
	{
		return build_expression<std::multiplies>(m0, m1);
	}


	//m*m
	template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
	auto operator*(const m0_t& m0 , const m1_t& m1)
	{

		constexpr int R = std::decay_t<m0_t>::row_num;
		constexpr int C = std::decay_t<m0_t>::col_num;
		constexpr int R1 = std::decay_t<m1_t>::row_num;
		constexpr int C1 = std::decay_t<m1_t>::col_num;

		static_assert(C == R1);

		using T = std::decay_t<m0_t>::type;
		using T1 = std::decay_t<m1_t>::type;
		static_assert(std::is_same_v<T, T1>);

		return matrix_imp::free_expression<T, R, C1, m0_t, m1_t, matrix_imp::mat_x_mat>(m0, m1, matrix_imp::mat_x_mat{});
	}

	//m/m
	template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
	auto operator/(const m0_t& m0, const m1_t& m1)
	{
		return matrix_imp::LU_solver::LU_inverse(m0, m1);
	}

	//m/=m
	template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
	void operator/=(m0_t&& m0, const m1_t& m1)
	{
		std::forward<m0_t>(m0) = matrix_imp::LU_solver::LU_inverse(m0, m1);
	}

	//m+=m
	template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
	void operator+=(m0_t&& m0, const m1_t& m1)
	{
		std::forward<m0_t>(m0) = build_expression<std::plus>(std::forward<m0_t>(m0), m1);
	}

	//m-=m
	template< matrix_imp::matrix_like m0_t, matrix_imp::matrix_like m1_t>
	void operator-=(m0_t&& m0, const m1_t& m1)
	{
		std::forward<m0_t>(m0) = build_expression<std::minus>(std::forward<m0_t>(m0), m1);
	}

	//m+=s
	template< matrix_imp::matrix_like m0_t, typename m1_t>
	void operator+=(m0_t&& m0, const m1_t& m1)
	{
		std::forward<m0_t>(m0) = build_expression<std::plus>(std::forward<m0_t>(m0), m1);
	}

	//m-=s
	template< matrix_imp::matrix_like m0_t, typename m1_t>
	void operator-=(m0_t&& m0, const m1_t& m1)
	{
		std::forward<m0_t>(m0) = build_expression<std::minus>(std::forward<m0_t>(m0), m1);
	}

	//m*=s
	template< matrix_imp::matrix_like m0_t, typename m1_t>
	void operator*=(m0_t&& m0, const m1_t& m1)
	{
		std::forward<m0_t>(m0) = build_expression<std::multiplies>(std::forward<m0_t>(m0), m1);
	}


	//m/=s
	template< matrix_imp::matrix_like m0_t, typename m1_t>
	void operator/=(m0_t&& m0, const m1_t& m1)
	{
		std::forward<m0_t>(m0) = build_expression<std::divides>(std::forward<m0_t>(m0), m1);
	}

	// cross
	template< matrix_imp::matrix_like m_t >
	auto cross(const m_t& a,const m_t& b)
	{
		constexpr int R = std::decay_t<m_t>::row_num;
		constexpr int C = std::decay_t<m_t>::col_num;
		using T = std::decay_t<m_t>::type;

		static_assert(R == 3);
		static_assert(C == 1);

		matrix<T, R, C> ret;
		ret(0) = a(1) * b(2) - a(2) * b(1);
		ret(1) = a(2) * b(0) - a(0) * b(2);
		ret(2) = a(0) * b(1) - a(1) * b(0);

		return ret;

	};


	template< matrix_imp::matrix_like m_t >
	auto det(const m_t& a)
	{
		constexpr int R = std::decay_t<m_t>::row_num;
		constexpr int C = std::decay_t<m_t>::col_num;
		using T = std::decay_t<m_t>::type;

		static_assert(R == C);

		if constexpr (R == 1)
		{
			return matrix_imp::accessor::apply(a, 0, 0);
		}
		else
		{
			T ret{};
			for (int i = 0; i < R; i++)
			{
				int s = i % 2 == 0 ? 1 : -1;
				ret += s * a(0, i) * det(cofactor(a, 0, i));
			}
			return ret;
		}

	}

	// length
	template< matrix_imp::matrix_like m0_t >
	auto length(const m0_t& v)
	{
		using T = std::decay_t<m0_t>::type;
		T ret = transpose(v) * v;
		return std::sqrt(ret);
	};


	template<typename T,int N>
	auto identity()
	{
		matrix<T, N, N> ret;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (i == j)
				{
					ret(i, j) = 1;
				}
				else
				{
					ret(i, j) = 0;
				}
			}
		}
		return ret;
	}
}
