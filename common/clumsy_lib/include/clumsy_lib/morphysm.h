
#pragma once

#include "clumsy_lib/dynamic_type_map.h"
#include "clumsy_lib/type_getter.h"

#include <unordered_map>
#include <functional>

namespace clumsy_lib
{
	template<typename Interface, typename Enum_T = void, typename = void >
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

	template<typename Interface, typename Enum_T>
	class Morphysm <Interface, Enum_T, std::enable_if_t< std::is_enum_v<Enum_T> > >
	{
	public:
		template<typename Enum_T>
		void shift(Enum_T enum_v)
		{
			m_shifter[enum_v]();
		}

		template<typename Sub_T>
		void register_sub_type(Enum_T enum_v)
		{
			m_morphysm.register_sub_type<Sub_T>();
			m_shifter[enum_v] = [this]()
				{
					m_morphysm.shift<Sub_T>();
				};
		}

		Interface* operator->()
		{
			return m_morphysm.operator->();
		}

	private:
		std::unordered_map<Enum_T, std::function<void()>> m_shifter;

		Morphysm<Interface> m_morphysm;
	};

}