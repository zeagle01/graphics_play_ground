
#pragma once

#include <map>
#include <string>
#include <memory> 
#include <vector>
#include <type_traits>

#define ADD_MEMBER_POINTER(name,type) struct name: type_map::variable<type> {}; name* name_var;

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
		}

	private:
		std::map<std::string, std::shared_ptr<void>> m_datas;

	};
}

