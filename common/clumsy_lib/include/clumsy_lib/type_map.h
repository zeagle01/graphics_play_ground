
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <set>
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


	template<typename Base_Type = void>
	class Type_Map 
	{
	public:
		template<typename Sub_Type>
		void add_type(std::shared_ptr<Sub_Type> a)
		{
			//TODO: sub class check;
			{
				auto key = typeid(Sub_Type).name();
				if (!type_map.count(key))
				{
					type_map[key] = std::static_pointer_cast<Base_Type>(a);
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
				type_map[key] = std::static_pointer_cast<Base_Type>(obj);
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

		/////////////////// for loop
		auto begin() { return type_map.begin(); } 
		auto end() { return type_map.end(); }
	private:
		std::map<std::string, std::shared_ptr<Base_Type>> type_map;
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
		const T& get() const
		{
			return data;
		}

		T& get_ref()
		{
			return data;
		}
	};


	//////////////////////////////////////////////////
	template<typename type_map_t>
	class Variable_Accecor
	{
	public:
		void set_type_map(std::shared_ptr<type_map_t> type_map)
		{
			m_type_map = type_map;
		}

		template<typename Variable_Type>
		auto& get_ref_value()
		{
			auto variable = m_type_map->get_type<Variable_Type>();
			return variable->get_ref();
		}

		template<typename Variable_Type>
		const auto& get_value()  const
		{
			auto variable = m_type_map->get_type<Variable_Type>();
			return variable->get();
		}

		template<typename Variable_Type, typename Value_Type = Variable_Type::value_type>
		void set_value(const Value_Type& d)
		{
			//static_assert(std::is_convertible_v<Value_Type, Variable_Type::value_type>);
			auto variable = m_type_map->get_type<Variable_Type>();
			if (!variable)
			{
				m_type_map->add_type<Variable_Type>();
			}
			variable->set(d);
		}
	private:
		std::shared_ptr<type_map_t> m_type_map;
	};

	template<typename type_map_t,typename Dependent_Variables>
	class Variable_Accecor_With_Constriant :public  Variable_Accecor<type_map_t>
	{
	public:
		template<typename Variable_Type>
		requires clumsy_lib::Type_In_List<Variable_Type, Dependent_Variables>
		const auto& get_value()
		{
			return Variable_Accecor<type_map_t>::template get_value<Variable_Type>();
		}

		template<typename Variable_Type, typename Value_Type = Variable_Type::value_type>
		requires clumsy_lib::Type_In_List<Variable_Type, Dependent_Variables>
		void set_value(const Value_Type& d)
		{
			Variable_Accecor<type_map_t>::template set_value<Variable_Type>(d);
		}
	};

	template<typename Dependent_Variables>
	using Variable_Acc_Constraint = Variable_Accecor_With_Constriant<Type_Map<void>, Dependent_Variables>;

	using Variable_Acc = Variable_Accecor<Type_Map<void>>;


}