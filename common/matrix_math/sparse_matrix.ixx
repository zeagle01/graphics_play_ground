
module;

#include <vector>
#include <functional>
#include <map>
#include <set>

export module matrix_math;


namespace matrix_math
{

	export template<typename mat_type,typename vec_type>
	class linear_system
	{
	public:
		// num of unknown as vec
		void set_variables_num(int n)
		{
			m_mat.clear();
			m_rhs.assign(n, {});
			m_rhs.shrink_to_fit();
		}
		void set_fixed_variables(const std::vector<int>& fixed_vars)
		{
			m_fixed_vars = std::set<int>(fixed_vars.begin(), fixed_vars.end());
		}



		struct writter
		{
			std::map<int, std::map<int, mat_type>>& mat;
			const std::set<int>& fixed_vars;
			int ri;
			int ci;

			writter& operator+=(const mat_type& v) 
			{
				if (!fixed_vars.contains(ri))
				{
					mat[ri][ci] += v;
				}
				return *this; 
			}

			writter& operator=(const mat_type& v) 
			{
				if (!fixed_vars.contains(ri))
				{
					mat[ri][ci] = v;
				}
				return *this; 
			}
		};

		struct rhs_writter
		{
			std::vector<vec_type> &b;
			const std::set<int>& fixed_vars;
			int i;

			rhs_writter& operator+=(const vec_type& v) 
			{
				if (!fixed_vars.contains(i))
				{
					b[i] = b[i] + v;
				}
				return *this; 
			}

			rhs_writter& operator=(const vec_type& v) 
			{
				if (!fixed_vars.contains(i))
				{
					b[i] = v;
				}
				return *this; 
			}

		};

		using get_rhs_writer = std::function<rhs_writter (int si, int i)>;
		using get_lhs_writer = std::function<writter (int si, int i, int j)>;
		using get_element_equations = std::function<void(get_lhs_writer, get_rhs_writer, int)>;

		std::function<void(get_element_equations)> get_write_loop(int stencil_num, std::function<std::vector<int>(int)> get_stencil) 
		{

			get_lhs_writer get_lhs = [&, get_stencil](int si, int i, int j)->writter
				{
					std::vector<int> stencil = get_stencil(si);
					int vi = stencil[i];
					int vj = stencil[j];
					return  writter{ m_mat, m_fixed_vars,vi, vj };
				};

			get_rhs_writer get_rhs = [&, get_stencil](int si, int i )->rhs_writter
				{
					std::vector<int> stencil = get_stencil(si);
					int vi = stencil[i];
					return  rhs_writter{ m_rhs, m_fixed_vars,vi };
				};



			auto ret = [&, get_lhs, get_rhs, stencil_num ](get_element_equations fn)
				{
					for (int si = 0; si < stencil_num; si++)
					{
						fn(get_lhs, get_rhs, si);
					}
				};
			return ret;
		}


		void for_each_nz(std::function<void(int row, int col, const mat_type& v)> fn) const
		{
			for (const auto& rowIt : m_mat)
			{
				int row = rowIt.first;
				for (const auto& colIt : m_mat.at(row))
				{
					int col = colIt.first;
					const mat_type& value = colIt.second;
					fn(row, col, value);
				}
			}
		}

		void for_each_variables(std::function<void(int vi, const mat_type& diag, const vec_type& rhs)> fn) const
		{
			for (int i = 0; i < m_rhs.size(); i++)
			{
				if (!m_fixed_vars.contains(i))
				{
					fn(i, m_mat.at(i).at(i), m_rhs[i]);
				}
			}
		}

	private:
		std::map<int, std::map<int, mat_type>> m_mat;
		std::vector<vec_type> m_rhs;
		std::set<int> m_fixed_vars;

	};



	export template<typename mat_type,typename vec_type>
	class Jacobi_solver
	{
	public:
		void solve(const linear_system<mat_type, vec_type>& linear_systems, std::vector<vec_type>& x)
		{
			int max_it = 30;
			for (int it = 0; it < max_it; it++)
			{
				std::vector<vec_type>  Ax(x.size());
				linear_systems.for_each_nz([&](int row, int col, const mat_type& m)
					{
						if (row != col)
						{
							Ax[row] = Ax[row] + m * x[col];
						}
					}
				);

				linear_systems.for_each_variables(
					[&](int v, const auto& diag, const auto& b)
					{
						x[v] = (b - Ax[v]) / diag;
					}
				);
			}
		}
	};


}
