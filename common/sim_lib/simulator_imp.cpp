
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"
#include "clumsy_lib/morphysm.h"
#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/macro_loop.h"

#include "matrix_math/matrix_math.h"


module sim_lib : simulator_imp;

import :sim_data;

import :simulator_datas;
import :simulator_data_update;

import :solver;
import :solver_dummy;



namespace sim_lib
{

	using namespace simulator_data_update;

	class simulator_imp
	{
	public:

		void init() 
		{ 
			m_solver.register_sub_type<dummy>(solver_type::Dummy);

			auto interface_dep_graph = clumsy_lib::Dependent_Graph::build<interface_var_list>();
			auto simulator_dep_graph = clumsy_lib::Dependent_Graph::build<simulator_var_list>();
			auto merged = clumsy_lib::Dependent_Graph::merge(interface_dep_graph, simulator_dep_graph);
			m_interface_data_propagator.set_dependent_graph(merged);
		}

		template<typename var>
		void set(typename const var::type& data)
		{
			m_interface_datas.get_ref<var>() = data;
			m_interface_data_propagator.touch<var>();
		}

		template<typename var>
		const typename var::type& get()
		{
			return m_interface_datas.get_ref<var>();
		}


		void step()
		{
			switch_solver();

			config_simulator_propagatro();

			update_data();

			m_simulator_data_propagator.propagate();

			m_solver->update_data(m_simulator_datas, m_simulator_data_propagator.get_all_change_status());

			m_simulator_data_propagator.clear_all_changes();

			m_interface_data_propagator.clear_all_changes();

			m_solver->solve();

			map_dynamic_positions_back_to_interface_positions::apply(
				m_interface_datas.get_ref<sim_data::positions>(), m_solver->get_result(),
				m_simulator_datas.get_ref<simulator_datas::dynamic_vert_index>()
			);
		}

	private:
		void switch_solver()
		{
			clumsy_lib::Down_Stream_Datas<interface_var_list> change_checker(m_interface_data_propagator.get_all_change_status());
			if (change_checker.is_changed<sim_data::solver>())
			{
				m_solver.shift(m_interface_datas.get_ref<sim_data::solver>());
			}
		}

		void config_simulator_propagatro()
		{
			clumsy_lib::Down_Stream_Datas<interface_var_list> change_checker(m_interface_data_propagator.get_all_change_status());
			if (change_checker.is_changed<sim_data::solver>())
			{
				auto simulator_dep_graph = clumsy_lib::Dependent_Graph::build<simulator_var_list>();
				auto solver_dep_graph = m_solver->compute_dep_graph();
				auto merged = clumsy_lib::Dependent_Graph::merge(simulator_dep_graph, solver_dep_graph);
				m_simulator_data_propagator.set_dependent_graph(merged);
				m_simulator_data_propagator.set_all_changes();
			}

		}

		void update_data()
		{
			clumsy_lib::dependent_updater<simulator_var_list>::apply(m_simulator_datas, m_interface_datas, m_interface_data_propagator.get_all_change_status());
		}
	private:

		using interface_var_list = clumsy_lib::extract_member_type_list_t<sim_data>;
		clumsy_lib::Static_Type_Map<interface_var_list> m_interface_datas;

		using simulator_var_list = clumsy_lib::extract_member_type_list_t<simulator_datas>;
		clumsy_lib::Static_Type_Map<simulator_var_list> m_simulator_datas;

		clumsy_lib::Dependent_Propagator<interface_var_list> m_interface_data_propagator;

		clumsy_lib::Dependent_Propagator<simulator_var_list> m_simulator_data_propagator;

		clumsy_lib::Morphysm<solver_base,solver_type> m_solver;

	};
}

