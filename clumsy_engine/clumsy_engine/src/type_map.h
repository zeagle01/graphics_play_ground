

#pragma once

#include <map>
#include <memory>


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
				//return dynamic_cast<Sub_T*>(type_map[key]);
			}
			else
			{
				auto ret = std::make_shared<Sub_T>();

				add(ret);

				return ret;
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

}
