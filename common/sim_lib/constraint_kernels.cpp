module;

#include <vector>

module sim_lib : constraint_kernels;


namespace sim_lib
{

	template<typename mat_type, typename vec_type>
	struct elemen_equations_std_vector
	{
		elemen_equations_std_vector(int num)
		{
			m_rhs.resize(num);
			m_rhs.shrink_to_fit();

			m_lhs.resize(num * num);
			m_lhs.shrink_to_fit();
		}

		int get_size() const
		{
			return m_rhs.size();
		}

		const mat_type& get_lhs(int row, int col) const
		{
			int n = m_rhs.size();
			return m_lhs[col * n + row];
		}

		void set_lhs(int row, int col,const mat_type& m) 
		{
			int n = m_rhs.size();
			 m_lhs[col * n + row] = m;
		}

		const vec_type get_rhs(int vi) const
		{
			return m_rhs[vi];
		}

		void set_rhs(int vi, const vec_type& v)
		{
			m_rhs[vi] = v;
		}

	private:
		std::vector<mat_type> m_lhs;
		std::vector<vec_type> m_rhs;
	};

	template<typename mat_type, typename vec_type>
	using element_equations = elemen_equations_std_vector<mat_type, vec_type>;

	struct inertial
	{

		template<typename mat_type, typename vec_type>
		static element_equations<mat_type, vec_type> compute_elemnt(float mass, float dt, const vec_type& gravity_accelaration, const vec_type& velocity)
		{
			element_equations<mat_type, vec_type> equations{1};

			float inertial_h = mass / dt / dt;
			equations.set_lhs(0, 0, inertial_h);
			equations.set_rhs(0, mass * gravity_accelaration + inertial_h * (velocity * dt));
			return equations;
		}

	};

}


