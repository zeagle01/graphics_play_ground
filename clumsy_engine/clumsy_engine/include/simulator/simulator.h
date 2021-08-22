

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>

#include "clumsy_lib/type_map.h"
#include "clumsy_lib/morphism_types.h"
#include "clumsy_lib/dependent_variable_set.h"
#include "Simulation_Data.h"
#include "system_equations_solver.h"
#include "linear_equations_solver.h"



namespace clumsy_engine
{

	class Interaction;


	class Simulator :public clumsy_lib::Variable_Accecor<clumsy_lib::Type_Map<void>>
	{
	public:

		Simulator();

		void update();


		template<typename Inter,typename ...P>
		void add_interaction(P&& ... p)
		{
			auto inter = std::make_shared<Inter>(std::forward<P>(p)...);

			m_interactions_map.add_type(inter);

			inter->set_variable_set(m_data_map);

		}

		template<typename Inter>
		void remove_interaction()
		{
			m_interactions_map.remove_type<Inter>();
		}

		template<typename T>
		void set_linear_solver()
		{
			m_linear_solver.set_current_type<T>();
		}


	private:

		void assemble_equations();

		clumsy_lib::Type_Map<Interaction> m_interactions_map;
		std::shared_ptr<clumsy_lib::Variable_Set> m_data_map;
		clumsy_lib::Morphism_Types<Linear_Equations_Solver_Base> m_linear_solver;

		std::vector<Element_Equation> m_equations;




	};

}



