
#pragma once

#include "type_map.h"
#include <memory>



namespace clumsy_lib
{
	template<typename  Interface_T >
	class Morphism_Types
	{
	public:
		template<typename Sub_Type>
		void set_current_type()
		{
			m_current_type = m_type_map.get_type<Sub_Type>();
		}

		template<typename Type_Group>
		void add_types()
		{
			m_type_map.add_types<Type_Group>();
		}

		std::shared_ptr<Interface_T> operator->()
		{
			return m_current_type;
		}

	private:
		Type_Map<Interface_T> m_type_map;
		std::shared_ptr<Interface_T> m_current_type;
	};

}
