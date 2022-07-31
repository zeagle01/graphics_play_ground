
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
			m_current_type.obj = m_type_map.get_as_interface_type<Sub_Type>();
			m_current_type.concret_type_name = std::string(typeid(Sub_Type).name());
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

			if (!m_current_type.obj)
			{
				m_current_type.obj = m_type_map.begin()->obj;
				m_current_type.concret_type_name = m_type_map.begin()->type_name;
			}
			return m_current_type.obj;
		}

		std::shared_ptr<Interface_T> get_curretn_type()
		{
			return m_current_type.obj;
		}

		std::string get_current_type_name()
		{
			return m_current_type.concret_type_name;
		}

	private:
		Type_Map<Interface_T, New_Type_Fn> m_type_map;
		struct InterfacePtr
		{
			std::shared_ptr<Interface_T> obj;
			std::string concret_type_name;
		};
		InterfacePtr m_current_type;

	};

	template<typename  Interface_T, typename U = void>
	class Morphism_Types :public Morphism_Types_Imp<Interface_T> { };

	template<typename  Interface_T >
	class Morphism_Types<Interface_T, std::void_t<typename Interface_T::new_type_fn> > 
		: public Morphism_Types_Imp<Interface_T,typename Interface_T::new_type_fn> { };





}
