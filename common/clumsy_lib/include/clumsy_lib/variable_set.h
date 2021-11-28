
#pragma once

#include <memory>
#include <vector>
#include "type_map.h"
#include "variable_accecor.h"
namespace clumsy_lib
{


	///////////////////////variable set
	template  <typename Type_Map,typename Variable_Accecor>
	class Variable_Set_Compose : public Type_Map, public Variable_Accecor
	{
	public:
		Variable_Set_Compose() 
		{
			auto shared_of_this_with_delete = std::shared_ptr<Variable_Set_Compose<Type_Map, Variable_Accecor>>(this, [](auto) {});
			this->set_type_map(shared_of_this_with_delete);
		}

		template< typename variables_struct>
		static auto build_variable_set()
		{

			using variables = clumsy_lib::extract_member_type_list_t<variables_struct>;

			std::shared_ptr<Variable_Set_Compose> ret = std::make_shared<Variable_Set_Compose>();

			for_each_type<variables, add_variable<Variable_Set_Compose> >(*ret);

			return ret;
		}

		template< typename variables_struct>
		static auto build_variable_set(variables_struct& my_struct)
		{

			using variables = clumsy_lib::extract_member_type_list_t<variables_struct>;

			std::shared_ptr<Variable_Set_Compose> ret = std::make_shared<Variable_Set_Compose>();

			for_each_type<variables, add_variable_with_obj<Variable_Set_Compose> >(*ret, my_struct);

			return ret;
		}

	private:

		template<typename variable_set>
		struct add_variable 
		{
			template<typename T>
			static void apply(variable_set& out)
			{
				out.add_type<T>();
			}
		};

		template<typename variable_set, typename variables_struct>
		struct add_variable_with_obj
		{
			template<typename T>
			static void apply(variable_set& out, variables_struct& my_struct)
			{
				out.add_type<T>();
			}
		};
	};

	using Variable_Set = Variable_Set_Compose<Type_Map<void>, Variable_Accecor<Type_Map<void>>>;



}
