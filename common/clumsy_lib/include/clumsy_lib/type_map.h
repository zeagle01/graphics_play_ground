
#pragma once

#include <memory>
#include <vector>
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
		Type_Map() :Get_Begin_End<Type_Map<Base_Type>>(*this) {}

		template<typename Sub_Type>
		void add_type(std::shared_ptr<Sub_Type> a)
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
		void add_type()
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				type_map[key] = Sub_Type{};
			}
		}

		template<typename Sub_Type>
		Sub_Type& get_type()
		{
			auto key = typeid(Sub_Type).name();
			if (!type_map.count(key))
			{
				add_type<Sub_Type>();
			}
			return std::any_cast<Sub_Type&>(type_map[key]);
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


	template<typename Variable_Set>
	class Variable_Accecor;

	///////////////////////variable set
	template<typename Variable_Base>
	class Variable_Set : public Type_Map<Variable_Base>, public Variable_Accecor<Variable_Set<Variable_Base>>
	{
	public:
		Variable_Set():Variable_Accecor<Variable_Set<Variable_Base>>(*this)  { }
	};



	////////////////build variable

	template<typename variable_set>
	struct add_variable 
	{
		template<typename T>
		static void apply(variable_set& out)
		{
			out.add_type<T>();
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








	//////////////////////////////////////////////////
	template<typename Variable_Set>
	class Variable_Accecor
	{
	public:
		Variable_Accecor(Variable_Set& varialbe_set) :m_varialbe_set(varialbe_set) {};

		template<typename Variable_Type>
		const auto& get_value()
		{
			auto variable = m_varialbe_set.get_type<Variable_Type>();
			return variable->get();
		}

		template<typename Variable_Type, typename Value_Type = Variable_Type::value_type>
		void set_value(const Value_Type& d)
		{
			static_assert(std::is_convertible_v<Value_Type, Variable_Type::value_type>);
			auto variable = m_varialbe_set.get_type<Variable_Type>();
			if (!variable)
			{
				add_variable<Variable_Type>();
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
		Variable_Accecor_With_Constriant(Variable_Set& varialbe_set) :m_variable_accecor(varialbe_set) {};

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

	class Tree_Node
	{
	public:

		void set_children_dirty(bool value)
		{
			for (auto c : m_children)
			{
				c->set_me_and_children_dirty(value);
			}
		}

		void set_me_and_children_dirty(bool value)
		{
			set_dirty(value);

			for (auto c : m_children)
			{
				c->set_me_and_children_dirty(value);
			}

		}

		void set_dirty(bool value)
		{
			m_dirty = value;
		}

		bool is_dirty()
		{
			return m_dirty;
		}

		void add_child(Tree_Node* n)
		{
			m_children.push_back(n);
		}

	private:
		std::vector<Tree_Node*> m_children;
		bool m_dirty = false;
	};


	struct Plain_Computer
	{
		template<typename data_acc,typename T>
		static void apply(data_acc& datas, T& d) {}
	};

	template<typename T, typename Variable_Base, typename Computer = Plain_Computer, typename dependent_types = clumsy_lib::type_list<>>
	class Dependent_Variable :public Tree_Node, public Variable_Base
	{

	public:
		using value_type = T;
		using dependent_variables = dependent_types;
		T data;
		Dependent_Variable() 
		{
			m_upstream_variables = std::make_shared<Variable_Set<Variable_Base>>();
			m_upstream_variables_acceccor = std::make_shared<Variable_Accecor_With_Constriant<Variable_Set<Variable_Base>, dependent_variables>>(*m_upstream_variables);
		}

		void set(const T& d)
		{
			data = d;

			set_dirty(false);

			set_children_dirty(true);

		}
		const T& get()
		{
			if (is_dirty())
			{
				Computer::apply(*m_upstream_variables_acceccor, data);
				set_dirty(false);
			}
			return data;
		}


		template<typename T>
		void add_upstream_variable(std::shared_ptr<T> upstream_variable)
		{
			upstream_variable->add_child(this);

			m_upstream_variables->add_type(upstream_variable);
		}

	protected:

		std::shared_ptr<Variable_Accecor_With_Constriant<Variable_Set<Variable_Base>, dependent_variables>> m_upstream_variables_acceccor;
		std::shared_ptr<Variable_Set<Variable_Base>> m_upstream_variables;
	};


	////////////////build dependent
	template< typename variable_type>
	struct build_dependent
	{
		template<typename dependent_variable_type>
		static void apply(auto& variables)
		{
			auto variable = variables.get_type<variable_type>();
			auto dependent_variable = variables.get_type<dependent_variable_type>();
			variable->add_upstream_variable(dependent_variable);
		}
	};

	template<typename tl,  template<typename U> typename F, typename ...P>
	static void for_each_depend_type(P&&... p)
	{
		if constexpr (!clumsy_lib::is_empty_v<tl>)
		{
			using current_t = clumsy_lib::front_t<tl>;

			using current_tl = typename current_t::dependent_variables;

			clumsy_lib::for_each_type<current_tl, F<current_t>>(std::forward<P>(p)...);

			using poped_list = clumsy_lib::pop_front_t<tl>;
			for_each_depend_type<poped_list, F>(std::forward<P>(p)...);
		}

	};

}