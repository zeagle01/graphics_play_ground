

#pragma once

#include <map>
#include <memory>
#include "type_list.h"


namespace clumsy_engine
{
	class Data_Base;

	template<typename Base_T>
	struct Type_Map
	{

		template<typename Sub_T>
		void add(std::shared_ptr<Sub_T> a)
		{
			//TODO: sub class check;
			{
				auto key = typeid(Sub_T).name();
				if (!type_map.count(key))
				{
					type_map[key] = a;
				}
			}
		}

		template<typename Sub_T>
		std::shared_ptr<Sub_T> get()
		{
			auto key = typeid(Sub_T).name();
			if (type_map.count(key))
			{
				return std::dynamic_pointer_cast<Sub_T>(type_map[key]);
			}
			else
			{

				return nullptr;
			}

		}

		auto begin()
		{
			return type_map.begin();
		}

		auto end()
		{
			return type_map.end();
		}

		std::map<std::string, std::shared_ptr<Base_T>> type_map;
	};

	//using Sim_Data_Ptr = std::shared_ptr<Type_Map<Data_Base>>;

	class Simulation_Datas
	{
	public:

		template<typename T>
		void add_sim_data(std::shared_ptr<T> sim_data)
		{
			m_sim_datas.add(sim_data);
		}

		template<typename T>
		auto get_sim_data() 
		{
			return m_sim_datas.get<T>();
		}

		template<typename T>
		const auto& get_data()
		{
			auto sim_data = m_sim_datas.get<T>();
			return sim_data->get();
		}



		template<typename T, typename dataT = T::data_type>
		void set_data(const dataT& d)
		{
			static_assert(std::is_convertible_v<dataT, T::data_type>);
			auto sim_data = m_sim_datas.get<T>();
			if (!sim_data)
			{
				sim_data = std::make_shared<T>();
				m_sim_datas.add(sim_data);
			}
			sim_data->set(d);
		}


		auto begin()
		{
			return m_sim_datas.begin();

		}

		auto end()
		{
			return m_sim_datas.end();
		}



	private:
		Type_Map<Data_Base> m_sim_datas;

	};



	template<typename tl>
	class Simulation_Data_Acc
	{
	public:
		using types = tl;


		void set_(std::shared_ptr<Simulation_Datas> sim_datas)
		{
			m_data_map = sim_datas;
		}

		template<typename T, typename dataT = T::data_type>
		requires Type_In_List<T,tl>
		void set(const dataT& d)
		{

			static_assert(std::is_convertible_v<dataT, T::data_type>);

			auto n0 = typeid(dataT).name();
			auto n1 = typeid(T::data_type).name();

			if (!m_data_map)
			{
				m_data_map = std::make_shared<Simulation_Datas>();
			}

			m_data_map->set_data<T, dataT>(d);
		}

		template<typename T>
		requires Type_In_List<T,tl>
		const auto& get()
		{
			return m_data_map->get_data<T>();
		}


	private:
		std::shared_ptr<Simulation_Datas >m_data_map;

	};


}
