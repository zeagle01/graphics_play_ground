
#include "clumsy_lib/dependent_propagator.h"


#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/static_type_map.h"

#include "gmock/gmock.h"

#include <vector>
#include <string>
#include <ranges>

using namespace testing;
using namespace clumsy_lib;


import clumsy_lib;

namespace dependent_propagator_test
{
	struct print_name
	{
		template<typename T, typename ...dep>
		static void apply(std::vector<std::string >& out)
		{
			out.push_back(get_name<T>());
			(out.push_back(get_name<dep>()), ...);
		}

		template<typename T>
		static std::string get_name()
		{
			std::string ret = typeid(T).name();
			auto it = ret.find_last_of(':');
			ret = ret.substr(it + 1);
			return ret;
		}
	};

	struct vars0
	{
		CE_ENTRY(A, CE_LIST(deps));
		CE_ENTRY(B, CE_LIST(deps, A));
	};


	struct vars1
	{
		CE_ENTRY(A, CE_LIST(adjs))
			CE_ENTRY(B, CE_LIST(adjs, A))
			CE_ENTRY(C, CE_LIST(adjs, A, B))
			CE_ENTRY(D, CE_LIST(adjs, vars0::A))
			CE_ENTRY(E, CE_LIST(adjs, B))
	};

	struct vars2
	{
		CE_ENTRY(A, CE_USE(fn, print_name)		CE_LIST(adjs))
			CE_ENTRY(B, CE_USE(fn, print_name)		CE_LIST(adjs, A))
	};


	class static_dep_graph_test : public Test
	{
	public:
		void SetUp() override
		{
			using  adj_list = extract_member_type_list_t<vars0>;
			m_dep_graph0.build<adj_list >();

			using  adj_list_1 = extract_member_type_list_t<vars1>;
			m_dep_graph1.build<adj_list_1, get_adjs>();

		}

	protected:
		template<typename T>
		struct get_adjs { using type = typename T::adjs; };

		template<typename T>
		struct get_fn { using type = typename T::fn; };

		template<typename T>
		struct always_update { static bool apply(...) { return true; } };


		static_dep_graph m_dep_graph0;
		static_dep_graph m_dep_graph1;
	};

	TEST_F(static_dep_graph_test, build_dep_graph)
	{
		EXPECT_TRUE((m_dep_graph0.has_edge<vars0::A, vars0::B>()));
	}

	TEST_F(static_dep_graph_test, build_dep_graph_with_adj_getter)
	{
		EXPECT_TRUE((m_dep_graph1.has_edge<vars1::A, vars1::C>()));
	}

	TEST_F(static_dep_graph_test, merged_dep_graph)
	{
		auto dep_grpha_merged = static_dep_graph::merge(m_dep_graph0, m_dep_graph1);

		EXPECT_TRUE((dep_grpha_merged.has_edge<vars0::A, vars1::D>()));

	}

	TEST_F(static_dep_graph_test, dynamic_walk)
	{

		dynamic_walker walker(m_dep_graph0);

		std::map<std::type_index, int> count;

		auto key_A = std::type_index(typeid(vars0::A));
		auto key_B = std::type_index(typeid(vars0::B));
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

	TEST_F(static_dep_graph_test, static_walk)
	{
		using list = extract_member_type_list_t<vars2>;

		static_walker<list, get_adjs> walker;
		std::vector<std::string > act;
		walker.walk<get_fn, always_update>(act);

		std::vector<std::string > exp{ "A","B","A" };
		EXPECT_THAT(act, Eq(exp));
	}

	template<typename T>
	struct update_var2_B_only
	{
		static bool apply(std::vector<std::string >& p)
		{
			if constexpr (std::is_same_v < T, vars2::B>)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
	};

	TEST_F(static_dep_graph_test, static_walk_partialy_update)
	{
		using list = extract_member_type_list_t<vars2>;

		static_walker<list, get_adjs> walker;
		std::vector<std::string > act;
		walker.walk<get_fn, update_var2_B_only>(act);

		std::vector<std::string > exp{ "B","A" };
		EXPECT_THAT(act, Eq(exp));
	}


}