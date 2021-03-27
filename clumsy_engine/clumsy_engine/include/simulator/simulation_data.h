

#pragma once

#include <vector>
#include "signal.h" 
#include "type_map.h"
#include "type_list.h"
#include "class_reflect.h"
#include "data_computes.h"
#include "matrix_math/matrix_math.h"

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

		void add_child(Node* n)
		{
			m_children.push_back(n);
		}

	private:
		std::vector<Node*> m_children;
		bool m_dirty = false;
	};

	template<typename T, typename Computer = Plain_Computer, typename dependent_types = type_list<>>
	class Dependent_Data :public Node, public Data_Base
	{

	public:
		using data_type = T;
		using dependent_variables = dependent_types;
		T data;
		Dependent_Data() 
		{
			m_senders = std::make_shared<Simulation_Datas>();
			m_sender_acc.set_(m_senders);
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

				Computer::apply(m_sender_acc, data);
				set_dirty(false);
			}
			return data;
		}


		template<typename T>
		void add_sender(std::shared_ptr<T> sender)
		{
			sender->add_child(this);

			m_senders->add_sim_data(sender);
		}

	protected:

		Simulation_Data_Acc<dependent_variables> m_sender_acc;
		std::shared_ptr<Simulation_Datas> m_senders;
	};


	using empty_deps = type_list<>;

	struct Plain_Computer
	{
		template<typename data_acc,typename T>
		static void apply(data_acc& datas, T& d) {}
	};




#define TMP(...) __VA_ARGS__ 

#define DEF_MEM(x,t,c,tl) \
		class x :public Dependent_Data<t,c, tl > {};\
		x* x##_var;\



	struct data
	{
		using vf = std::vector<float>;
		using vi = std::vector<int>;

		using v_v3f = std::vector<vec3f>;
		using v_v3i = std::vector<vec3i>;

		DEF_MEM(Gravity,					vec3f,		Plain_Computer,										empty_deps);
		DEF_MEM(Time_Step,					float,		Plain_Computer,										empty_deps);
		DEF_MEM(Mass_Density,				float,		Plain_Computer,										empty_deps);
		DEF_MEM(Stretch_Stiff,				float,		Plain_Computer,										empty_deps);
		DEF_MEM(Position,					v_v3f,		Plain_Computer,										empty_deps);
		DEF_MEM(Triangle_Indice,			vi,			Plain_Computer,										empty_deps);

		DEF_MEM(Vertex_Num,					int,		TMP(Get_List_Size<Position, 1>),					type_list<Position>);
		DEF_MEM(Velocity,					v_v3f,		TMP(Allocate_With_Size<v_v3f, Vertex_Num, 1>),		type_list<Vertex_Num>);
		DEF_MEM(Last_Frame_Position,		v_v3f,		TMP(Allocate_With_Size<v_v3f, Vertex_Num, 1>),		type_list<Vertex_Num>);
		DEF_MEM(Triangle_Area,				vf,			Compute_Triangle_Area,								TMP(type_list<Position,Triangle_Indice>));
		DEF_MEM(Vertex_Area,				vf,			Compute_Vertex_Area,								TMP(type_list<Vertex_Num,Triangle_Area,Triangle_Indice>));
		DEF_MEM(Mass,						vf,			Compute_Mass,										TMP(type_list<Mass_Density,Vertex_Area>));

		DEF_MEM(Edge_Indice,				vi,			Compute_Edge_Indices,								TMP(type_list<Triangle_Indice, Vertex_Num>));
		DEF_MEM(Edge_Length,				vf,			Compute_Edge_Length,								TMP(type_list<Position, Edge_Indice>));
		DEF_MEM(Delta_Position,				v_v3f,		Delta,												TMP(type_list< Position,Last_Frame_Position> ));
	} ;

	template<typename ...P>
	struct typesss
	{
		typesss(std::tuple<P...> t) {};
		using types = type_list<P...>;
	};


	template<typename tup>
	struct extract_tuple_pointer;

	template<typename ...P>
	struct extract_tuple_pointer<std::tuple<P*...>>
	{
		using types = type_list<P...>;
	};

	using tuple_t = decltype(as_tuple(std::declval<data>()));
	using all_types = extract_tuple_pointer<tuple_t>::types;





	template<typename tl,  template<typename U> typename F, typename ...P>
	static void for_each_depend_type(P&&... p)
	{
		if constexpr (!is_empty<tl>)
		{
			using current_t = front<tl>;

			using current_tl = typename current_t::dependent_variables;

			for_each_type<current_tl, F<current_t>>(std::forward<P>(p)...);

			using poped_list = pop_front<tl>;
			for_each_depend_type<poped_list, F>(std::forward<P>(p)...);
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