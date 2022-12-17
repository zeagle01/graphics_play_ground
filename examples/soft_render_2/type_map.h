
#pragma once

#include <map>
#include <string>
#include <memory> 
#include <type_traits>

namespace soft_render
{

	class type_map
	{
	public:

		template<typename variable_type, variable_type... values >
		struct variable
		{
			using type = variable_type;

			constexpr  static variable_type get_default_value()
			{
				if constexpr (sizeof...(values) == 0)
				{
					return {};
				}
				else if constexpr (sizeof...(values) == 1)
				{
					return (values, ...);
				}
				else
				{
					static_assert(false, "shouldn't be more default values size greater than one !");
					return {};
				}
			}
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

		template<typename Var>
		void add_type()
		{
			auto key = typeid(Var).name();
			m_datas[key] = std::make_shared<Var::type>(Var::get_default_value());
		}

	private:

	private:
		std::map<std::string, std::shared_ptr<void>> m_datas;

	};
}
