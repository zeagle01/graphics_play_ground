

#pragma once



#include <vector>
#include <memory>
#include <map>
#include <string>


namespace clumsy_engine
{

	class Interaction;
	class System_Equations_Solver;
	class Simulation_Data;

	class Simulator
	{


	public:
		Simulator();
		void set_mesh(std::vector<float> positions, std::vector<int> triangles);



		void update();

		std::vector<float> get_delta_positions() ;

		template<typename Inter,typename ...P>
		void add_interaction(P&& ... p)
		{
			auto inter = std::make_unique<Inter>(std::forward<P>(p)...);

			m_interactions_map.add(inter.get());

			m_interations.push_back(std::move(inter));
		}




	private:
		void add_interaction(std::unique_ptr<Interaction> interaction);



	private:

		std::vector<std::unique_ptr<Interaction>> m_interations;

		std::shared_ptr<Simulation_Data> m_sim_data;


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

		//std::unique_ptr<System_Equations_Solver> m_solver;


	};

}



