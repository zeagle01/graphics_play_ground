
#include "clumsy_lib/dependent_propagator.h"


#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/static_type_map.h"

#include "gmock/gmock.h"

using namespace testing;
using namespace clumsy_lib;


import clumsy_lib;

namespace dependent_propagator_test
{

	struct my_data
	{
		CE_ADD_NODE(A, CE_TYPE(int) CE_FIELD(deps, type_list<>));
		CE_ADD_NODE(B, CE_TYPE(int) CE_FIELD(deps, type_list<A>));

		CE_ADD_NODE(C, CE_TYPE(int) CE_FIELD(deps, type_list<>));

		CE_ADD_NODE(D, CE_TYPE(int));
		CE_ADD_NODE(E, CE_TYPE(int) CE_FIELD(deps, type_list<D>));

		CE_ADD_NODE(F, CE_TYPE(int));
	};

	struct assign
	{
		template<typename T, typename U>
		static void apply(T& out, const U& in)
		{
			out = in;
		}
	};

	struct down_stream_datas
	{
		CE_ADD_NODE(A, CE_TYPE(int) CE_FIELD(deps, type_list<my_data::A>));
		CE_ADD_NODE(B, CE_TYPE(int) CE_FIELD(deps, EVAL(type_list<my_data::B, A>)));
		CE_ADD_NODE(C, CE_TYPE(int)	CE_FIELD(dep_update_fn, assign) CE_FIELD(deps, type_list<my_data::F>));

	};

	class Dependent_Propagator_Test :public Test
	{
	protected:
		using  up_stream_list = extract_member_type_list_t<my_data>;
		using  down_stream_list = extract_member_type_list_t<down_stream_datas>;
		void SetUp() override
		{
			auto up_deps = Dependent_Graph::build<up_stream_list>();
			auto down_deps = Dependent_Graph::build<down_stream_list>();
			auto deps = Dependent_Graph::merge(up_deps, down_deps);
			m_up_stream.set_dependent_graph(deps);
		}

		Dependent_Propagator<up_stream_list> m_up_stream;
	};


//	TEST_F(Dependent_Propagator_Test, touched_is_changed)
//	{
//		m_up_stream.touch<my_data::A>();
//		EXPECT_TRUE(m_up_stream.is_changed<my_data::A>());
//	}
//
//	TEST_F(Dependent_Propagator_Test, is_changed_is_affacted_by_deps)
//	{
//		m_up_stream.touch<my_data::A>();
//		EXPECT_TRUE(m_up_stream.is_changed<my_data::B>());
//	}
//
//	TEST_F(Dependent_Propagator_Test, is_changed_only_affacts_related)
//	{
//		m_up_stream.touch<my_data::A>();
//		EXPECT_FALSE(m_up_stream.is_changed<my_data::C>());
//	}
//
//	TEST_F(Dependent_Propagator_Test, var_name_dont_even_have_to_have_deps)
//	{
//		m_up_stream.touch<my_data::D>();
//		EXPECT_TRUE(m_up_stream.is_changed<my_data::E>());
//	}
//
//	TEST_F(Dependent_Propagator_Test, down_stream_is_affacted)
//	{
//		m_up_stream.touch<my_data::A>();
//
//		using  down_stream_list = extract_member_type_list_t<down_stream_datas>;
//		Down_Stream_Datas<down_stream_list> down_stream(m_up_stream.get_all_change_status());
//		EXPECT_TRUE(down_stream.is_changed<down_stream_datas::A>());
//		EXPECT_TRUE(down_stream.is_changed<down_stream_datas::B>());
//	}
//
//	TEST_F(Dependent_Propagator_Test, update_fn_of_down_stream_is_executed)
//	{
//
//		Static_Type_Map<down_stream_list> obj;
//		Static_Type_Map<up_stream_list> upstream_obj;
//
//		int exp = 42;
//		upstream_obj.get_ref<my_data::F>() = exp;
//		m_up_stream.touch<my_data::F>();
//
//		using  down_stream_list = extract_member_type_list_t<down_stream_datas>;
//		Down_Stream_Datas<down_stream_list> down_stream(m_up_stream.get_all_change_status());
//
//		dependent_updater<down_stream_list>::apply(obj, upstream_obj, m_up_stream.get_all_change_status());
//
//		EXPECT_THAT(obj.get_ref<down_stream_datas::C>(),Eq(exp));
//	}

	namespace _2
	{
		struct dep_table
		{
			CE_ENTRY(A, CE_LIST(deps));
			CE_ENTRY(B, CE_LIST(deps, A));
		};

		TEST(static_dep_graph_test, build_dep_graph)
		{
			using config_table = dep_table;
			using  adj_list = extract_member_type_list_t<config_table>;
			static_dep_graph dep_graph;

			dep_graph.build<adj_list >();// build default getter

			EXPECT_TRUE((dep_graph.has_edge<config_table::A, config_table::B>()));
		}

		struct dep_table_2
		{
			CE_ENTRY(A, CE_LIST(adjs));
			CE_ENTRY(B, CE_LIST(adjs, A));
			CE_ENTRY(C, CE_LIST(adjs, A, B));
			CE_ENTRY(D, CE_LIST(adjs, dep_table::A));
		};

		template<typename T>
		struct get_adjs
		{
			using type = typename T::adjs;
		};

		TEST(static_dep_graph_test, build_dep_graph_with_adj_getter)
		{
			using config_table = dep_table_2;
			using  adj_list = extract_member_type_list_t<config_table>;
			static_dep_graph dep_graph;
			dep_graph.build<adj_list, get_adjs>();
			EXPECT_TRUE((dep_graph.has_edge<config_table::A, config_table::C>()));
		}

		TEST(static_dep_graph_test, merged_dep_graph)
		{

			using  adj_list0 = extract_member_type_list_t<dep_table>;
			static_dep_graph dep_graph0;
			dep_graph0.build<adj_list0 >();

			using  adj_list1 = extract_member_type_list_t<dep_table_2>;
			static_dep_graph dep_graph1;
			dep_graph1.build<adj_list1, get_adjs >();

			auto dep_grpha_merged = static_dep_graph::merge(dep_graph0, dep_graph1);

			EXPECT_TRUE((dep_grpha_merged.has_edge<dep_table::A, dep_table_2::D>()));

		}

		TEST(static_dep_graph_test, walk)
		{

			using config_table = dep_table;
			using  adj_list = extract_member_type_list_t<config_table>;
			static_dep_graph dep_graph;

			dep_graph.build<adj_list >();// build default getter

			dynamic_walker walker(dep_graph);

			std::map<std::type_index, int> count;

			auto key_A = std::type_index(typeid(dep_table::A));
			auto key_B = std::type_index(typeid(dep_table::B));
			count.insert({ key_A,0 });
			count.insert({ key_B,0 });

			walker.walk(key_A, [&](auto type_key)
				{
					count[type_key]++;
				}
			);

			EXPECT_THAT(count[key_A], Eq(1));
			EXPECT_THAT(count[key_B], Eq(1));

		}

	}

}