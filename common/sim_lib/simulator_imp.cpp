
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"
#include "clumsy_lib/get_instance.h"
#include "clumsy_lib/morphysm.h"

#include "matrix_math/matrix_math.h"


module sim_lib : simulator_imp;

import :sim_data;
import :solver;


using namespace sim_lib;
REGISTER_INSTANCE_FACTORY(solver_base, dummy, solver_type::Dummy);

namespace sim_lib
{


	template<typename T, int N>
	static void convert(std::vector<std::array<T, N>>& out, const std::vector<matrix_math::mat<N, 1, T>>& in)
	{
		out.resize(in.size());
		for (int i = 0; i < in.size(); i++)
		{
			for (int j = 0; j < N; j++)
			{
				out[i][j] = in[i](j);
			}
		}

	}

	class simulator_imp
	{
	public:

		void init() 
		{ 
			m_sim_deps = clumsy_lib::Dependent_Graph::build<var_list>();
			m_propagator.set_dependent_graph(m_sim_deps);

			m_solver.register_sub_type<dummy>(solver_type::Dummy);
		}

		template<typename var>
		void set(typename const var::type& data)
		{
			m_datas.get_ref<var>() = data;
			m_propagator.touch<var>();
		}

		template<typename var>
		const typename var::type& get()
		{
			return m_datas.get_ref<var>();
		}


		void step()
		{
			clumsy_lib::Down_Stream_Datas<var_list> change_checker(m_propagator.get_all_change_status());

			if (change_checker.is_changed<sim_data::solver>())
			{
				m_solver.shift(m_datas.get_ref<sim_data::solver>());

				auto solver_deps = m_solver->compute_dep_graph();
				auto all_deps = clumsy_lib::Dependent_Graph::merge(m_sim_deps, solver_deps);
				m_propagator.set_dependent_graph(all_deps);
				m_propagator.set_all_changes();
			}

			m_solver->update_data(m_datas, m_propagator.get_all_change_status());

			m_propagator.clear_all_changes();

			m_solver->solve();

			convert(m_datas.get_ref<sim_data::positions>(), m_solver->get_result());
		}

	private:
		using var_list = clumsy_lib::extract_member_type_list_t<sim_data>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		clumsy_lib::Dependent_Propagator<var_list> m_propagator;
		clumsy_lib::adj_list_t m_sim_deps;

		clumsy_lib::Morphysm<solver_base,solver_type> m_solver;

	};
}

