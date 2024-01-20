
module;


#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"
#include "clumsy_lib/morphysm.h"
#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/macro_loop.h"
#include "clumsy_lib/type_list.h"

#include "matrix_math/matrix_math.h"

#include <memory>
#include <span>

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
		bool contains() const 
		{
			return m_graph.contains<var>();
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
		void set_partially(const std::vector<int>& indices, const auto& data)
		{
			if (indices.size() == data.size())
			{
				auto& var_data = get<var>();

				for (int i = 0; i < indices.size(); i++)
				{
					int index = indices[i];
					var_data[index] = data[i];
				}

				mark_changed<var>();
			}
			else
			{
				printf(" %s partially set size don't match\n", typeid(var).name());
				m_all_data_is_valid = false;
			}
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

		void commit_all_changes()
		{
			m_changes_has_commited = true;
		}

		bool step()
		{
			bool data_is_valid = validate_interface_data();
			if (!data_is_valid)
			{
				return false;
			}

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

			return true;
		}

	private:

		bool validate_interface_data()
		{
			if (m_changes_has_commited)
			{
				printf(" ---- new data commited \n");
				m_changes_has_commited = false;

				m_all_data_is_valid = true;

				using interface_var_list_new = clumsy_lib::extract_member_type_list_t<sim_data>;
				clumsy_lib::static_walker<interface_var_list_new, get_validator_params> walker;

				walker.walk<validator_dispatchor, update_when_dirty>(m_all_data_is_valid, m_interface_datas, m_interface_data_status);
			}
			return m_all_data_is_valid;
		}

		void propagate_simulator_changes()
		{
			clumsy_lib::For_Each_Type<simulator_var_list>::apply<touch_simulator_var>(m_simulator_data_status, m_interface_data_status);
		}

		struct touch_simulator_var
		{
			template<typename var >
			static void apply(change_status& down_stream, const change_status& up_stream)
			{
				if (down_stream.contains<var>())
				{
					if (up_stream.is_changed<var>())
					{
						down_stream.touch<var>();
					}
				}
				else
				{
					//printf("%s do not affact solver  \n", typeid(var).name());
				}
			}
		};

		void print_change_status()
		{
			//for (const auto& it : m_interface_data_status.get_all_change_status())
			//{
			//	if (it.second)
			//	{
			//		printf("data changed: %s \n", it.first.name());
			//	}
			//}

			for (const auto& it : m_simulator_data_status.get_all_change_status())
			{
				if (it.second)
				{
					printf("data changed: %s \n", it.first.name());
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
				m_simulator_data_status.set_dependent_graph(m_solver->get_dep_graph());

				m_simulator_data_status.set_all_changes(true);

				m_interface_data_status.set_all_changes(true);

			}

		}

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
		bool m_changes_has_commited = false;
		bool m_all_data_is_valid = false;

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

		//validatros
	private:
		struct validator_dispatchor
		{
			template<typename var, typename ...param>
			static void apply(bool& all_data_valid, const clumsy_lib::Static_Type_Map<interface_var_list>& interface_datas, const change_status& interface_data_status)
			{
				if constexpr (!std::is_same_v<typename var::validator, clumsy_lib::type_list<>>)
				{
					invoke_all<var,typename var::validator>::apply(all_data_valid, interface_datas);
				}


				//printf(" invalid data: %s --\n", typeid(var).name());
				//(printf(" invalid data: %s \n", typeid(param).name()), ...);
			}

			template<typename var, typename fn_tl>
			struct invoke_all;

			template<typename var, typename ...validator>
			struct invoke_all<var, clumsy_lib::type_list<validator...>>
			{
				static void apply(bool& all_data_valid, const clumsy_lib::Static_Type_Map<interface_var_list>& interface_datas)
				{
					(invoke_one<typename validator::name, var, typename validator::params>::apply(all_data_valid, interface_datas),...);

				}


				template<typename Fn, typename var, typename params>
				struct invoke_one;

				template<typename Fn, typename var, typename ...param>
				struct invoke_one<Fn, var, clumsy_lib::type_list<param...>>
				{
					static void apply(bool& all_data_valid, const clumsy_lib::Static_Type_Map<interface_var_list>& interface_datas)
					{
						all_data_valid = Fn::apply(interface_datas.get_ref<var>(), interface_datas.get_ref<param>()...);

						if (!all_data_valid)
						{
							printf(" invalid data: %s, violation: %s\n", typeid(var).name(), typeid(Fn).name());
						}
					}

				};

			};
		};

		 


		template<typename var>
		struct get_validator_params
		{

			template<typename valids>
			struct project_params;

			template<typename ...valid>
			struct project_params<clumsy_lib::type_list<valid...>>
			{
				using type = clumsy_lib::type_list<typename valid::params...>;
			};

			template<typename valids>
			struct merge_params
			{
				using params_tl = project_params<valids>::type;
				using type = clumsy_lib::merge_list_imp<params_tl>::type;
			};

			template<typename U>
			static auto  choosor()
			{
				if constexpr (requires {typename U::validator; })
				{
					if constexpr (std::is_same_v<U::validator, clumsy_lib::type_list<>>)
					{
						return clumsy_lib::type_list<>{};
					}
					else
					{
						return typename merge_params<typename U::validator>::type{};
					}
				}
				else
				{
					return clumsy_lib::type_list<>{};
				}
			}

			using type = decltype(choosor<var>());
		};

		template<typename var>
		struct update_when_dirty
		{
			static bool apply(bool& all_data_valid, const clumsy_lib::Static_Type_Map<interface_var_list>& interface_datas, const change_status& interface_data_status)
			{
				return all_data_valid && interface_data_status.is_changed<var>();
			}
		};

	};



}

