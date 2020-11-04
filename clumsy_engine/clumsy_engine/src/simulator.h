

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>
#include <typeinfo>

#include "data_connections.h"
#include "type_map.h"



namespace clumsy_engine
{

	class Interaction;
	class System_Equations_Solver;
	class Data_Base;


	class Simulator
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

			//Data_Connections::setup_data_connections(this, inter.get());

			inter->set_up_data<Inter::dependent_variables>(m_data_map);

		}

	public:



		std::vector<float> get_edge_lengths();
		std::vector<float> get_delta_positions() ;


		template<typename T, typename dataT = T::data_type>
		void set(const dataT& d)
		{
			static_assert(std::is_convertible_v<dataT, T::data_type>);

			auto n0 = typeid(dataT).name();
			auto n1 = typeid(T::data_type).name();

			m_data_map.set_data<T, dataT>(d);
		}

		template<typename T>
		const auto& get()
		{
			return m_data_map.get_data<T>();
		}


	private:



		Type_Map<Interaction> m_interactions_map;
		Simulation_Datas m_data_map;


	};

}



