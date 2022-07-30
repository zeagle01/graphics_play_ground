
#pragma once

#include "variable_set.h"
#include "variable_accecor.h"
#include "static_loop.h"
#include "raw_pointer.h"

namespace clumsy_lib
{

	class Tree_Node
	{
	public:
		void add_child(Tree_Node* n)
		{
			m_children.push_back(n);
		}


		/////////
		void set_is_changed_by_parent(bool v)
		{
			m_is_changed_by_parent = v;
		}

		void set_is_changed_by_self(bool v)
		{
			m_is_changed_by_self = v;
		}

		void set_children_is_changed_by_parent(bool v)
		{
			for (auto c : m_children)
			{
				c->set_is_changed_by_parent(v);
				c->set_children_is_changed_by_parent(v);
			}
		}

		bool is_changed_by_parent()
		{
			return m_is_changed_by_parent;
		}

		bool is_changed_by_self()
		{
			return m_is_changed_by_self;
		}


	private:
		std::vector<Tree_Node*> m_children;
		bool m_is_changed_by_self = false;
		bool m_is_changed_by_parent = false;
	};


	struct Plain_Computer
	{
		template<typename data_acc, typename T>
		static void apply(data_acc& datas, T& d) {}
	};

	template<typename T, typename Computer = Plain_Computer, typename dependent_types = clumsy_lib::type_list<>>
	class Dependent_Variable :public Tree_Node
	{

	public:
		using value_type = T;
		using dependent_variables = dependent_types;
		T data;
		Raw_Pointer<T> device_pointer;
		Dependent_Variable()
		{
			m_upstream_variables = std::make_shared<Variable_Set>();
			m_upstream_variables_acceccor = std::make_shared<Variable_Accecor_With_Constriant<Variable_Set, dependent_variables>>();
			m_upstream_variables_acceccor->set_type_map(m_upstream_variables);
		}

		void set(const T& d)
		{
			data = d;
			set_is_changed_by_self(true);
			set_children_is_changed_by_parent(true);
		}

		T& get_ref()
		{
			check_n_upate();
			return data;
		}

		template<typename Device_Type >
		Raw_Pointer<T> get_device_pointer(Device_Type device)
		{
			check_n_upate();

			if (m_device_poiter_is_dirty)
			{
				device->upload<T>(device_pointer, data);
				m_device_poiter_is_dirty = false;
			}

			return device_pointer;
		}

		const T& get()
		{
			return get_ref();
		}

		template<typename T>
		void add_upstream_variable(std::shared_ptr<T> upstream_variable)
		{
			upstream_variable->add_child(this);

			m_upstream_variables->add_type(upstream_variable);
		}
	private:
		bool check_n_upate()
		{
			bool changed = true;

			if (is_changed_by_self())
			{
				set_is_changed_by_self(false);
				set_is_changed_by_parent(false);
			}
			else if (is_changed_by_parent())
			{
				Computer::apply(*m_upstream_variables_acceccor, data);
				set_is_changed_by_parent(false);
			}
			else
			{
				changed = false;
			}

			if (changed)
			{
				m_device_poiter_is_dirty = true;
			}

			return changed;
		}


	protected:

		std::shared_ptr<Variable_Accecor_With_Constriant<Variable_Set, dependent_variables>> m_upstream_variables_acceccor;
		std::shared_ptr<Variable_Set> m_upstream_variables;
	private:
		bool m_device_poiter_is_dirty = true;
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

	template<typename tl, template<typename U> typename F, typename ...P>
	static void for_each_depend_type(P&&... p)
	{
		if constexpr (!clumsy_lib::is_empty_v<tl>)
		{
			using current_t = clumsy_lib::front_t<tl>;

			using current_tl = typename current_t::dependent_variables;

			clumsy_lib::For_Each_Type<current_tl>::template apply<F<current_t>>(std::forward<P>(p)...);

			using poped_list = clumsy_lib::pop_front_t<tl>;
			for_each_depend_type<poped_list, F>(std::forward<P>(p)...);
		}
	};


	template< typename Variable_Set, typename variables_struct>
	static auto build_dependent_variable_set()
	{
		auto varialbe_set = Variable_Set::template build_variable_set< variables_struct >();
		using variables = clumsy_lib::extract_member_type_list_t<variables_struct>;
		for_each_depend_type<variables, build_dependent>(*varialbe_set);
		return varialbe_set;
	}

}
