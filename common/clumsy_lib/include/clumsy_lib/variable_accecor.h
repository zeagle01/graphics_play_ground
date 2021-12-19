
#pragma once

#include "type_map.h"

namespace clumsy_lib
{
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
			static_assert(std::is_convertible_v<Value_Type, Variable_Type::value_type>);
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
