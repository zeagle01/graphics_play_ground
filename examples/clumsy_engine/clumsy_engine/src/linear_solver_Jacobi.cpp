

#include "linear_solver_Jacobi.h"
#include "profiler.h"
#include "compressed_sparse_row.h"
#include "interaction.h"

#include <vector>


namespace clumsy_engine
{

	template<typename T, int N>
	std::vector<vec<N, T>> diagonal_inverse(const CSR<mat<N, N, T>>& A, const std::vector<vec<N, T>>& v)
	{
		std::vector<vec<N, T>> ret(v.size());
		for (int i = 0; i < v.size(); i++)
		{
			if (i == 0)
			{
				ret[i] = get_uniform<N, 1, T>(0);
				continue;
			}

			int rowBegin = A.I[i];
			int colIndex = A.J[rowBegin];
			ret[i] = v[i] / A.values[rowBegin];
		}
		return ret;
	}


	std::vector<vec3f> Linear_Solver_Jacobi::solve(const std::vector<vec3f>& x0, std::vector<Element_Equation> const& equations)
	{
		RECORD_FUNCTION_DURATION();

		std::vector<vec3f> ret = x0;
		CSR<mat3x3f> A;
		std::vector<vec3f> b(x0.size(), get_uniform<3, 1, float>(0));
		convert_system_equations_to_CSR(A, b, equations);

		int iter_name = 50;
		for (int i = 0; i < iter_name; i++)
		{
			auto r = b - A * ret;
			ret = ret + diagonal_inverse(A, r);
		}
		return ret;
	}

	/////////////////////////////////////////////
	/////////////////////////////////////////////
	struct Sparse_Matrix_ELL
	{

		void assemble(const std::vector<Element_Equation_Pointer>& element_equation_pointers, int vertex_num)
		{

			A_buffer.resize(vertex_num * max_rows);
			i_buffer.resize(vertex_num * (max_rows + 1));
			b_buffer.resize(vertex_num);

			indices = Array2D<int >{ i_buffer.data(),vertex_num };
			values = Array2D<mat3x3f >{ A_buffer.data(),vertex_num };
			b = b_buffer.data();


			m_vertex_num = vertex_num;
			for (const auto& eq : element_equation_pointers)
			{
				add_to_ell(eq);
			}
		}

		void add_to_ell(const Element_Equation_Pointer& eq)
		{
			const int n = eq.vertex_num_per_element;
			for (int si = 0; si < eq.element_count; si++)
			{
				mat3x3f* element_A = eq.A + si * n * n;
				vec3f* element_b = eq.b + si * n;

				auto element_index = &eq.element_indices[si * n];

				for (int vi = 0; vi < n; vi++)
				{
					for (int vj = 0; vj < n; vj++)
					{
						int ind_i = element_index[vi];
						int ind_j = element_index[vj];

						int found_pos = find(&indices(1, ind_i), max_rows, ind_j);
						if (found_pos == -1)
						{
							int end_pos = indices(0, ind_i)++; //atomic

							indices(end_pos + 1, ind_i) = ind_j;

							values(end_pos, ind_i) += element_A[vi * n + vj]; //atomic
						}
						else if (found_pos < max_rows)
						{
							indices(found_pos + 1, ind_i);
							values(found_pos, ind_i) = element_A[vi * n + vj];
						}
						else
						{
							//report error
						}
					}
				}
			}
		}



	private:

		int find(int* d, int n, int target)
		{
			for (int i = 0; i < n; i++)
			{
				if (d[n] == target)
				{
					return n;
				}
			}
			return -1;
		}

		template<typename T>
		struct Array2D
		{
			T* data;
			int col;

			T& operator()(int ri, int ci)
			{
				return data[ri * col + ci];
			}

			const T& operator()(int ri, int ci) const
			{
				return data[ri * col + ci];
			}
		};

		Array2D<int > indices;
		Array2D<mat3x3f > values; // A
		vec3f* b;
		int m_vertex_num;
		static constexpr int max_rows = 32;
		//buffer
		std::vector<mat3x3f> A_buffer;
		std::vector<int> i_buffer;
		std::vector<vec3f> b_buffer;
	};

	void Linear_Solver_Jacobi::solve(vec3f* x, const std::vector<Element_Equation_Pointer>& element_equation_pointers, int vertex_num)
	{
		Sparse_Matrix_ELL ell;
		ell.assemble(element_equation_pointers, vertex_num);

	}

}

