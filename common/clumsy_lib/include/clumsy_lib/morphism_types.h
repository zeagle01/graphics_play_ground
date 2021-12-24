
#pragma once

#include "type_map.h"
#include <memory>
#include <map>
#include <string>



namespace clumsy_lib
{


	template<typename  Interface_T, typename New_Type_Fn = New_Std_Shared_Ptr>
	class Morphism_Types_Imp
	{
	public:
		template<typename Sub_Type>
		void set_current_type()
		{
			m_current_type = m_type_map.get_as_interface_type<Sub_Type>();
		}

		template<typename Type_Group>
		void add_types()
		{
			m_type_map.add_types<Type_Group>();
		}

		std::shared_ptr<Interface_T> operator->()
		{
			//assert(!m_type_map.empty());

			if (m_type_map.empty())
			{
				return nullptr;
			}

			if (!m_current_type)
			{
				m_current_type = *m_type_map.begin();

			}
			return m_current_type;
		}

	private:
		Type_Map<Interface_T, New_Type_Fn> m_type_map;
		std::shared_ptr<Interface_T> m_current_type;
	};

	template<typename  Interface_T, typename U = void>
	class Morphism_Types :public Morphism_Types_Imp<Interface_T> { };

	template<typename  Interface_T >
	class Morphism_Types<Interface_T, std::void_t<typename Interface_T::new_type_fn> > 
		: public Morphism_Types_Imp<Interface_T,typename Interface_T::new_type_fn> { };





}
