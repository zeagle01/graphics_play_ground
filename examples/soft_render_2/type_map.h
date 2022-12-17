
#pragma once

#include <map>
#include <string>
#include <memory> 
#include <vector>
#include <type_traits>

namespace soft_render
{

	class type_map
	{
	public:

		template<typename variable_type>
		struct variable
		{
			using type = variable_type;
		};


		template<typename group>
		void build_from_groups()
		{

		}

		template<typename Var>
		auto& get_ref()
		{
			auto key = typeid(Var).name();
			return *(std::static_pointer_cast<Var::type>(m_datas[key]));
		}

		template<typename Var,typename ...P>
		void add_type(P&&... p)
		{
			auto key = typeid(Var).name();

			std::shared_ptr<Var::type> obj;
			if constexpr (std::is_aggregate_v<Var::type>)
			{
				obj = std::make_shared<Var::type>();
				typename Var::type& value = *obj;
				//value = Var::type{ ( std::forward<P>(p),... ) };
				value = typename Var::type { std::forward<P>(p)... };
			}
			else
			{
				obj = std::make_shared<Var::type>(std::forward<P>(p)...);
			}

			m_datas[key] = obj;
			m_ordered_datas.push_back(obj);
		}


		auto begin()
		{
			return m_ordered_datas.begin();
		}

		auto end()
		{
			return m_ordered_datas.end();
		}

	private:
		std::map<std::string, std::shared_ptr<void>> m_datas;
		std::vector<std::shared_ptr<void>> m_ordered_datas; // to keep insert order

	};


}
