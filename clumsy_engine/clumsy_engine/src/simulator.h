

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>
#include <typeinfo>

#include "data_connections.h"
//#include "simulation_data.h"


namespace clumsy_engine
{

	class Interaction;
	class System_Equations_Solver;
	class Simulation_Data;
	class Data_Wrapper;


	class Simulator
	{
		friend Data_Connections;
	public:

		Simulator();

		void update();


		template<typename Inter,typename ...P>
		void add_interaction(P&& ... p)
		{
			auto inter = std::make_unique<Inter>(std::forward<P>(p)...);

			m_interactions_map.add(inter.get());

			Data_Connections::setup_data_connections(this, inter.get());

			m_interations.push_back(std::move(inter));
		}

	public:

		template<typename T>
		void add_data()
		{
			auto data = std::make_unique<T>();

			m_data_map.add(data.get());

			m_datas.push_back(std::move(data));
		}

		void set_mesh(std::vector<float> positions, std::vector<int> triangles);

		std::vector<float> get_edge_lengths();
		std::vector<float> get_delta_positions() ;


		template<typename T, typename dataT = T::data_type>
		void set(const dataT& d)
		{
			static_assert(std::is_same_v<dataT, T::data_type>);
			auto data = m_data_map.get<T>();
			data->set(d);
		}

		template<typename T>
		const auto& get()
		{
			auto data = m_data_map.get<T>();
			using ret_type = decltype(data->get());
			//			std::string  n1 = typeid(ret_type).name();
			//			std::string n2 = typeid(T::data_type).name();
			static_assert(std::is_same_v<ret_type, const T::data_type&>);
			return data->get();
		}

	private:


		template<typename Base_T>
		struct Type_Map
		{


			template<typename Sub_T>
			void add(Sub_T* a)
			{
				//TODO: sub class check;
				{
					auto key = typeid(Sub_T).name();
					type_map[key] = a;
				}
			}

			template<typename Sub_T>
			Sub_T* get()
			{
				auto key = typeid(Sub_T).name();
				if (type_map.count(key))
				{
					return dynamic_cast<Sub_T*>(type_map[key]);
				}
				else
				{
					return nullptr;
				}

			}
			std::map<std::string, Base_T*> type_map;

		};

		Type_Map<Interaction> m_interactions_map;
		Type_Map<Data_Wrapper> m_data_map;

		std::vector<std::unique_ptr<Data_Wrapper>> m_datas;
		std::vector<std::unique_ptr<Interaction>> m_interations;

		std::shared_ptr<Simulation_Data> m_sim_data;
	};

}



