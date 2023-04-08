
#pragma once

#include <vector>
#include <memory>
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/static_loop.h"


namespace clumsy_lib
{
	template<typename tl>
	class Static_Type_Map
	{
	public:

		Static_Type_Map()
		{
			For_Each_Type<tl>::apply<Make_Shared>(this);
		}

		template<typename var_name>
		auto& get_ref() requires is_in<var_name,tl>
		{
			int i = get_index_v< tl, var_name>;
			auto ptr = std::static_pointer_cast<var_name::type>(m_datas[i]);
			return *ptr;
		}

		template<typename var_name>
		const auto& get_ref() const requires is_in<var_name,tl>
		{
			int i = get_index_v< tl, var_name>;
			auto ptr = std::static_pointer_cast<var_name::type>(m_datas[i]);
			return *ptr;
		}

	private:
		struct Make_Shared
		{
			template<typename var_name>
			static void apply(Static_Type_Map* obj)
			{
				int i = get_index_v< tl, var_name>;
				obj->m_datas[i] = std::make_shared<typename var_name::type>();
			}

		};

	private:
		static constexpr inline int  N = get_size_v<tl>;
		std::shared_ptr<void> m_datas[N];
	};

}
