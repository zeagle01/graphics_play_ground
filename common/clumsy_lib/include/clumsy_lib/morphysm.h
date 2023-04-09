
#pragma once

#include "clumsy_lib/dynamic_type_map.h"
#include "clumsy_lib/type_getter.h"

namespace clumsy_lib
{
	template<typename Interface>
	class Morphysm 
	{
	public:
		template<typename Sub_T>
		void shift()
		{
			m_current = &(m_data.get_ref<Sub_T>());
		}

		template<typename Sub_T>
		void register_sub_type()
		{
			m_data.add_or_replace<Sub_T>();
		}

		Interface* operator->()
		{
			return m_current;
		}

	private:
		Interface* m_current;
		Dynamic_Type_Map<Get_Its_Own_As_Type> m_data;
	};

}