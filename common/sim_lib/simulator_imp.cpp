
module;


#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"
#include "clumsy_lib/morphysm.h"
#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/macro_loop.h"

#include "matrix_math/matrix_math.h"

#include <memory>

module sim_lib : simulator_imp;

import :sim_data;

import :simulator_datas;
import :simulator_data_update;

import :solver;
import :solver_dummy;



namespace sim_lib
{
	using namespace simulator_data_update;

	class change_status
	{
	public:
		void set_dependent_graph(const clumsy_lib::static_dep_graph& graph)
		{
			m_graph = graph;

			m_walker = std::make_unique<clumsy_lib::dynamic_walker>(m_graph);

			init_all_changes(false);
		}

		template<typename var>
		void touch()
		{
			auto k = std::type_index(typeid(var));

			m_walker->walk(k, [this](auto node_key) 
				{
					m_status[node_key] = true;
				});
		}

		template<typename var>
		bool is_changed() const
		{
			auto k = std::type_index(typeid(var));
			return m_status.at(k);
		}

		void set_all_changes(bool v)
		{
			for (auto& it : m_status)
			{
				it.second = v;
			}
		}

		const std::unordered_map<std::type_index, bool>& get_all_change_status() const
		{
			return m_status;
		}

	private:
		void init_all_changes(bool v)
		{
			m_walker->walk([this,v](auto node_key)
				{
					m_status[node_key] = v;
				}
			);
		}

		std::unique_ptr<clumsy_lib::dynamic_walker> m_walker;
		std::unordered_map<std::type_index, bool> m_status;
		clumsy_lib::static_dep_graph m_graph;

	};

	class simulator_imp
	{
	public:
		simulator_imp()
		{
			clumsy_lib::For_Each_Type<interface_var_list>::apply<set_interface_init_value>(m_interface_datas);
		}


		void init() 
		{
			m_solver.register_sub_type<dummy_solver>(solver_type::Dummy);

			clumsy_lib::static_dep_graph interface_dep_graph;
			clumsy_lib::static_dep_graph simulator_dep_graph;

			interface_dep_graph.build<interface_var_list>();
			simulator_dep_graph.build<simulator_var_list>();

			auto merged = clumsy_lib::static_dep_graph::merge(interface_dep_graph, simulator_dep_graph);

			m_interface_data_status.set_dependent_graph(merged);
			m_interface_data_status.set_all_changes(true);
		}

		template<typename var>
		void set(const auto& data)
		{
			get<var>() = data;
			mark_changed<var>();
		}

		template<typename var>
		const typename auto& get() const
		{
			return m_interface_datas.get_ref<var>();
		}

		template<typename var>
		typename auto& get()
		{
			return m_interface_datas.get_ref<var>();
		}

		template<typename var>
		void mark_changed()
		{
			m_interface_data_status.touch<var>();
		}

		bool commit_all_changes()
		{
			using updater = clumsy_lib::dependent_updater<
				interface_var_list,
				clumsy_lib::default_get_deps,
				get_validator
			>;

			updater::apply (m_interface_datas, m_interface_datas, m_interface_data_status.get_all_change_status());

			bool all_data_valid = true;

			//clumsy_lib::For_Each_Type<interface_var_list>::apply<check_interface_data_valid>(all_data_valid, m_interface_datas);

			return all_data_valid;
		}

		void step()
		{
			switch_solver();

			config_simulator_propagatro();

			update_data();

			propagate_simulator_changes();

			m_solver->update_data(m_simulator_datas, m_simulator_data_status.get_all_change_status());

			print_change_status();

			m_simulator_data_status.set_all_changes(false);

			m_interface_data_status.set_all_changes(false);

			m_solver->solve();

			//output positions
			simulator_data_update::assign::apply(get<sim_data::positions>(), m_solver->get_result());
		}
	private:
		void propagate_simulator_changes()
		{
			clumsy_lib::For_Each_Type<simulator_var_list>::apply<touch_simulator_var>(m_simulator_data_status);
		}

		struct touch_simulator_var
		{
			template<typename var >
			static void apply(change_status& propagator)
			{
				if (propagator.is_changed<var>())
				{
					propagator.touch<var>();
				}
			}
		};

		void print_change_status()
		{
			for (const auto& it : m_interface_data_status.get_all_change_status())
			{
				if (it.second)
				{
					printf(" %s \n", it.first.name());
				}
			}

		}

	private:
		void switch_solver()
		{
			clumsy_lib::Down_Stream_Datas<interface_var_list> change_checker(m_interface_data_status.get_all_change_status());
			if (change_checker.is_changed<sim_data::solver>())
			{
				m_solver.shift(get<sim_data::solver>());
			}
		}

		void config_simulator_propagatro()
		{
			clumsy_lib::Down_Stream_Datas<interface_var_list> change_checker(m_interface_data_status.get_all_change_status());
			if (change_checker.is_changed<sim_data::solver>())
			{
				clumsy_lib::static_dep_graph simulator_dep_graph;
				simulator_dep_graph.build<interface_var_list>();

				clumsy_lib::static_dep_graph solver_dep_graph = m_solver->get_dep_graph();

				auto merged = clumsy_lib::static_dep_graph::merge(simulator_dep_graph, solver_dep_graph);

				m_simulator_data_status.set_dependent_graph(merged);

				m_simulator_data_status.set_all_changes(true);

				m_interface_data_status.set_all_changes(true);

			}

		}

	private:
		struct validator
		{
			template<typename T,typename ...U>
			static void apply(T& me,const U& ...deps)
			{
				bool deps_is_all_valid = true;
				deps_is_all_valid = (deps.is_valid&&...);
				if (deps_is_all_valid)
				{
					//me.is_valid = T::validator::apply(me.data, deps.data...);

					//printf(" %s exe validator \n", typeid(T::type).name());
				}
				else
				{
					//me.is_valid = false;
				}
			}

		};

		template<typename T>
		struct get_validator
		{
			using type = validator;
		};

		struct check_interface_data_valid
		{
			template<typename var,typename Obj>
			static void apply(bool& all_data_valid, Obj& obj)
			{
				all_data_valid = all_data_valid && obj.get_ref<var>().is_valid;
				if (!obj.get_ref<var>().is_valid)
				{
					printf("invalid %s  \n", typeid(var).name());
				}
			}
		};

	private:
		struct get_sim_data_ref
		{
			template<typename var, typename Obj>
			static auto& apply(Obj& obj)
			{
				return obj.template get_ref<var>();
			}

		};

		void update_data()
		{
			clumsy_lib::dependent_updater<simulator_var_list>
				::apply<
				clumsy_lib::Static_Type_Map<simulator_var_list>,
				clumsy_lib::Static_Type_Map<interface_var_list>,
				clumsy_lib::default_get_data_ref,
				get_sim_data_ref
				>
				(m_simulator_datas, m_interface_datas, m_interface_data_status.get_all_change_status());
		}
	private:

		using interface_var_list = clumsy_lib::extract_member_type_list_t<sim_data>;
		clumsy_lib::Static_Type_Map<interface_var_list> m_interface_datas;

		using simulator_var_list = clumsy_lib::extract_member_type_list_t<simulator_datas>;
		clumsy_lib::Static_Type_Map<simulator_var_list> m_simulator_datas;

		change_status m_interface_data_status;
		change_status m_simulator_data_status;

		clumsy_lib::Morphysm<solver_base,solver_type> m_solver;

	private:
		struct set_interface_init_value
		{
			template<typename T>
			static void apply(clumsy_lib::Static_Type_Map<simulator_imp::interface_var_list>& interface_datas)
			{
				if constexpr (!std::is_same_v<typename T::init_val, std::nullptr_t>)
				{
					interface_datas.get_ref<T>() = T::init_val::value;
				}
			}
		};

	};



}

