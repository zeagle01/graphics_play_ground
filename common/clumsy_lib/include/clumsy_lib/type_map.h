
#pragma once

#include <memory>
#include <vector>
#include <map>
#include "clumsy_lib/class_reflection.h"

namespace clumsy_lib
{

	//////////// type_map

	template<typename Contianer_T>
	class Get_Begin_End
	{
	public:
		Get_Begin_End(Contianer_T& container) :m_container(container) {}

		auto begin()
		{
			return m_container.begin();
		}

		auto end()
		{
			return m_container.end();
		}

	private:
		Contianer_T& m_container;
	};


	class Type_Map :public Get_Begin_End<Type_Map>
	{
	public:
		Type_Map() :Get_Begin_End<Type_Map>(*this) {}

		template<typename Sub_Type>
		void add_type(std::shared_ptr<Sub_Type> a)
		{
			//TODO: sub class check;
			{
				auto key = typeid(Sub_Type).name();
				if (!type_map.count(key))
				{
					type_map[key] = std::static_pointer_cast<void>(a);
				}
			}
		}

		template<typename Sub_Type>
		void add_type()
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				auto obj = std::make_shared<Sub_Type>();
				type_map[key] = std::static_pointer_cast<void>(obj);
			}
		}

		template<typename Sub_Type>
		std::shared_ptr<Sub_Type> get_type()
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				add_type<Sub_Type>();
			}
			return std::static_pointer_cast<Sub_Type>(type_map[key]);
		}

	private:
		std::map<std::string, std::shared_ptr<void>> type_map;
	};


	template<typename T>
	class Data_Holder
	{
	private:
		T data;
	public:
		Data_Holder() = default;

		using value_type = T;

		void set(const T& d)
		{
			data = d;
		}
		const T& get()
		{
			return data;
		}
	};


	//////////////////////////////////////////////////
	template<typename Variable_Set>
	class Variable_Accecor
	{
	public:
		Variable_Accecor(Variable_Set *varialbe_set) :m_varialbe_set(*varialbe_set) { };

		template<typename Variable_Type>
		const auto& get_value()
		{
			auto variable = m_varialbe_set.get_type<Variable_Type>();
			return variable->get();
		}

		template<typename Variable_Type, typename Value_Type = Variable_Type::value_type>
		void set_value(const Value_Type& d)
		{
			//static_assert(std::is_convertible_v<Value_Type, Variable_Type::value_type>);
			auto variable = m_varialbe_set.get_type<Variable_Type>();
			if (!variable)
			{
				m_varialbe_set.add_type<Variable_Type>();
			}
			variable->set(d);
		}
	private:
		Variable_Set& m_varialbe_set;
	};

	template<typename Variable_Set,typename Dependent_Variables>
	class Variable_Accecor_With_Constriant
	{
	public:
		template<typename Type_Map>
		Variable_Accecor_With_Constriant(Type_Map* type_map) :m_variable_accecor(type_map) {};

		template<typename Variable_Type>
		requires clumsy_lib::Type_In_List<Variable_Type, Dependent_Variables>
		const auto& get_value()
		{
			return m_variable_accecor.get_value<Variable_Type>();
		}

		template<typename Variable_Type, typename Value_Type = Variable_Type::value_type>
		requires clumsy_lib::Type_In_List<Variable_Type, Dependent_Variables>
		void set_value(const Value_Type& d)
		{
			m_variable_accecor.set_value<Variable_Type>(d);
		}

	private:
		Variable_Accecor<Variable_Set> m_variable_accecor;
	};

}