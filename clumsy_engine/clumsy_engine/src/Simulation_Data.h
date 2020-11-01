

#pragma once

#include <vector>
#include "signal.h" 
#include "type_map.h"
#include "type_list.h"

namespace clumsy_engine
{




	class Data_Base
	{
	public:
		virtual  ~Data_Base() {}
	};


	class Node
	{
	public:

		void set_children_dirty(bool value)
		{
			for (auto c : m_children)
			{
				c->set_me_and_children_dirty(dirty);
			}
		}

		void set_me_and_children_dirty(bool value)
		{
			set_dirty(value);

			for (auto c : m_children)
			{
				c->set_me_and_children_dirty(dirty);
			}

		}

		void set_dirty(bool value)
		{
			this->dirty = dirty;
		}

		bool is_dirty()
		{
			return dirty;
		}

		void add_child(Node* n)
		{
			m_children.push_back(n);
		}

	private:
		std::vector<Node*> m_children;
		bool dirty;
	};

	template<typename T, typename dependent_types = type_list<>>
	class Dependent_Data :public Node, public Data_Base
	{

	public:
		using data_type = T;
		using dependent_variables = dependent_types;
		T data;
		void set(const T& d)
		{
			data = d;

			set_children_dirty(true);

		}
		const T& get()
		{
			if (is_dirty())
			{
				compute(data);
				set_dirty(false);
			}
			return data;
		}


		template<typename T>
		void add_sender(std::shared_ptr<T> sender)
		{
			sender->add_child(this);

			m_senders.add_sim_data(sender);
		}

	protected:
		virtual void compute(T& d) {}
		Simulation_Datas m_senders;
	};




	///
namespace data
{


	//////
	class Gravity :public Dependent_Data<std::vector<float>> {};

	class Time_Step :public Dependent_Data<float> {};

	class Position :public Dependent_Data<std::vector<float>> {};

	class Velocity :public Dependent_Data<std::vector<float>> {};

	class Last_Frame_Position :public Dependent_Data<std::vector<float>> {};

	class Delta_Position :public Dependent_Data<std::vector<float>, type_list<Position, Last_Frame_Position>> {};

	class Mass :public Dependent_Data<std::vector<float>> {};

	class Triangle_Indices :public Dependent_Data<std::vector<int>> {};

	class Edge_Indice :public Dependent_Data<std::vector<int>>
	{
	public:
		void compute(data_type& edge_indice) override
		{

		}
	};

	class Edge_Length :public Dependent_Data<std::vector<float>>
	{
	public:
		void compute(data_type& edge_length) override
		{
			const auto& positions = m_senders.get_data<Position>();
			const auto& edge_indices = m_senders.get_data<Edge_Indice>();
			int eNum = edge_indices.size() / 2;
			edge_length.resize(eNum);
			for (int i = 0; i < eNum; i++)
			{
				int v0 = edge_indices[i * 2 + 0];
				int v1 = edge_indices[i * 2 + 1];
				float l = 0;
				for (int i = 0; i < 3; i++)
				{
					float d = positions[v0 * 3 + i] - positions[v1 * 3 + i];
					l += d * d;
				}
				edge_length[i] = std::sqrt(l);
			}
		}
	};


	using all_variables = type_list<Gravity, Time_Step, Position, Delta_Position, Velocity, Last_Frame_Position, Mass, Triangle_Indices, Edge_Indice>;
}

	///
	template<typename tl, typename F, typename ...P>
	static void for_each_type(P&&... p)
	{
		if constexpr (!is_empty<tl>)
		{
			using current_t = front<tl>;

			F::apply<current_t>(std::forward<P>(p)...);

			using poped_list = pop_front<tl>;

			for_each_type<poped_list, F>(std::forward<P>(p)...);
		}
		else
		{
			return;
		}

	};



//	template<typename tl0,typename tl1, typename F, typename ...P>
//	struct double_for_each_type
//	{
//		static void apply(P&&... p)
//		{
//
//		}
//	};

	template<typename tl,  template<typename U> typename F, typename ...P>
	static void for_each_type1(P&&... p)
	{
		if constexpr (!is_empty<tl>)
		{
			using current_t = front<tl>;

			using current_tl = typename current_t::dependent_variables;

			for_each_type<current_tl, F<current_t>>(std::forward<P>(p)...);

			using poped_list = pop_front<tl>;
			for_each_type1<poped_list, F>(std::forward<P>(p)...);
		}

	};

	template<typename tl0, typename tl1, typename F, typename ctl0 = tl0, typename ctl1 = tl1, typename ...P>
	static void double_for_each_type(P&&... p)
	{
		if constexpr (!is_empty<ctl0>&&!is_empty<ctl1>)
		{
			using current_t0 = front<ctl0>;

			using current_t1 = front<ctl1>;

			F::apply<current_t0, current_t1>(std::forward<P>(p)...);

			using poped_list0 = pop_front<ctl0>;
			using poped_list1 = pop_front<ctl1>;

			double_for_each_type<tl0, tl1, F, poped_list0, poped_list1 >(std::forward<P>(p)...);
		}
		else if constexpr (is_empty<tl0> && !is_empty<tl1>)
		{
			using current_t0 = front<tl0>;

			using current_t1 = front<ctl1>;

			F::apply<current_t0, current_t1>(std::forward<P>(p)...);

			using poped_list0 = pop_front<tl0>;
			using poped_list1 = pop_front<ctl1>;

			double_for_each_type<tl0, tl1, F, poped_list0, poped_list1>(std::forward<P>(p)...);
		}
		else 
		{
			return;
		}

	};

	struct extract_sim_data
	{
		template<typename T>
		static void apply(Simulation_Datas& subset_sim_datas, Simulation_Datas& all_sim_datas)
		{
			auto sim_data = all_sim_datas.get_sim_data<T>();
			subset_sim_datas.add_sim_data(sim_data);
		}
	};

	struct build_sim_data 
	{

		template<typename T>
		static void apply(Simulation_Datas& out)
		{
			out.add_sim_data<T>(std::make_shared<T>());

		}

	};


	template<typename sim_data_T>
	struct build_dependent
	{
		template<typename dependent_T>
		static void apply(Simulation_Datas& sim_datas)
		{
			auto sim_data = sim_datas.get_sim_data<sim_data_T>();
			auto dependent_data = sim_datas.get_sim_data<dependent_T>();
			sim_data->add_sender(dependent_data);
		}
	};
	












}