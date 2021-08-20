

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>

//#include "type_map.h"
#include "clumsy_lib/type_map.h"
#include "clumsy_lib/variable_set.h"
#include "Simulation_Data.h"
#include "system_equations_solver.h"



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

			inter->set_type_map(m_data_map);

		}

		template<typename Inter>
		void remove_interaction()
		{
			m_interactions_map.remove_type<Inter>();
		}


	private:

		void assemble_equations();

		clumsy_lib::Type_Map<Interaction> m_interactions_map;
		std::shared_ptr<clumsy_lib::Variable_Set> m_data_map;

		std::vector<Element_Equation> m_equations;


	};

}



