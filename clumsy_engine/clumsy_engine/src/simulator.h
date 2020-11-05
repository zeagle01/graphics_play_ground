

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>
#include <typeinfo>

#include "data_connections.h"
#include "type_map.h"
#include "Simulation_Data.h"



namespace clumsy_engine
{

	class Interaction;
	class System_Equations_Solver;
	class Data_Base;


	class Simulator:public Simulation_Data_Acc<all_types>
	{
		friend Data_Connections;
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

	public:



		std::vector<float> get_edge_lengths();
		std::vector<float> get_delta_positions() ;




	private:



		Type_Map<Interaction> m_interactions_map;
		std::shared_ptr< Simulation_Datas> m_data_map;


	};

}



