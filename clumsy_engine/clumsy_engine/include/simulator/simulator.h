

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>

#include "type_map.h"
#include "Simulation_Data.h"
#include "system_equations_solver.h"



namespace clumsy_engine
{

	class Interaction;


	class Simulator:public Simulation_Data_Acc<all_types>
	{
	public:

		Simulator();

		void update();


		template<typename Inter,typename ...P>
		void add_interaction(P&& ... p)
		{
			auto inter = std::make_shared<Inter>(std::forward<P>(p)...);

			m_interactions_map.add(inter);

			inter->set_(m_data_map);

		}


	private:

		void assemble_equations();


		Type_Map<Interaction> m_interactions_map;
		std::shared_ptr< Simulation_Datas> m_data_map;

		std::vector<Element_Equation> m_equations;


	};

}



