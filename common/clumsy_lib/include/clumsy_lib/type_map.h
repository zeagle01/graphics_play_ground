
#pragma once

#include <memory>
#include "clumsy_lib/class_reflection.h"

namespace clumsy_lib
{

	template<typename Base_Type>
	class Type_Map
	{
	public:
		template<typename Sub_Type>
		void add(std::shared_ptr<Sub_Type> a)
		{
			//TODO: sub class check;
			{
				auto key = typeid(Sub_Type).name();
				if (!type_map.count(key))
				{
					type_map[key] = a;
				}
			}
		}

		template<typename Sub_Type>
		void add()
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				type_map[key] = std::make_shared<Sub_Type>();
			}
		}

		template<typename Sub_Type>
		std::shared_ptr<Sub_Type> get()
		{
			auto key = typeid(Sub_Type).name();
			if (type_map.count(key))
			{
				return std::dynamic_pointer_cast<Sub_Type>(type_map[key]);
			}
			else
			{

				return nullptr;
			}

		}

		auto begin()
		{
			return type_map.begin();
		}

		auto end()
		{
			return type_map.end();
		}

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
		const T& get()
		{
			return data;
		}
	};



	template<typename Variable_Base>
	class Variable_Set
	{
	public:

		template<typename Variable_Type>
		void add_variable(std::shared_ptr<Variable_Type> variable)
		{
			m_variable_set.add(variable);
		}

		template<typename Variable_Type>
		void add_variable()
		{
			m_variable_set.add<Variable_Type>();
		}

		template<typename Variable_Type>
		auto get_variable() 
		{
			return m_variable_set.get<Variable_Type>();
		}

		template<typename Variable_Type>
		const auto& get_value()
		{
			auto variable = get_variable<Variable_Type>();
			return variable->get();
		}

		template<typename Variable_Type, typename Value_Type = Variable_Type::value_type>
		void set_value(const Value_Type& d)
		{
			static_assert(std::is_convertible_v<Value_Type, Variable_Type::value_type>);
			auto variable = get_variable<Variable_Type>();
			if (!variable)
			{
				add_variable<Variable_Type>();
			}
			variable->set(d);
		}

		auto begin()
		{
			return m_variable_set.begin();
		}

		auto end()
		{
			return m_variable_set.end();
		}

	private:
		Type_Map<Variable_Base> m_variable_set;

	};




	template<typename variable_set>
	struct add_variable 
	{
		template<typename T>
		static void apply(variable_set& out)
		{
			out.add_variable<T>();
		}
	};

	template<typename variables_struct,typename  variable_base>
	static auto build_variable_set()
	{
		using variable_set_t = Variable_Set<variable_base>;

		using variables = clumsy_lib::extract_member_type_list_t<variables_struct>;

		std::shared_ptr<variable_set_t> ret = std::make_shared<variable_set_t>();

		for_each_type<variables, add_variable<variable_set_t> >(*ret);

		return ret;
	}






}