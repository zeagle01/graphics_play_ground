
#include "clumsy_lib/dependent_propagator.h"


#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/class_reflection.h"

#include "gmock/gmock.h"

using namespace testing;
using namespace clumsy_lib;


namespace dependent_propagator_test
{

	struct my_data
	{
		CE_ADD_NODE(A, CE_FIELD(deps, type_list<>));
		CE_ADD_NODE(B, CE_FIELD(deps, type_list<A>));

		CE_ADD_NODE(C, CE_FIELD(deps, type_list<>));

		CE_ADD_NODE(D );
		CE_ADD_NODE(E, CE_FIELD(deps, type_list<D>));
	};

	struct down_stream_datas
	{
		CE_ADD_NODE(A, CE_FIELD(deps, type_list<my_data::A>));
		CE_ADD_NODE(B, CE_FIELD(deps, EVAL(type_list<my_data::B, A>)));

	};

	class Dependent_Propagator_Test :public Test
	{
	protected:
		using  up_stream_list = extract_member_type_list_t<my_data>;
		using  down_stream_list = extract_member_type_list_t<down_stream_datas>;
		void SetUp()override
		{
			auto up_deps = Dependent_Graph::build<up_stream_list>();
			auto down_deps = Dependent_Graph::build<down_stream_list>();
			auto deps = Dependent_Graph::merge(up_deps, down_deps);
			m_up_stream.set_dependent_graph(deps);
		}

		Dependent_Propagator<up_stream_list> m_up_stream;
	};


	TEST_F(Dependent_Propagator_Test, touched_is_changed)
	{
		m_up_stream.touch<my_data::A>();
		EXPECT_TRUE(m_up_stream.is_changed<my_data::A>());
	}

	TEST_F(Dependent_Propagator_Test, is_changed_is_affacted_by_deps)
	{
		m_up_stream.touch<my_data::A>();
		EXPECT_TRUE(m_up_stream.is_changed<my_data::B>());
	}

	TEST_F(Dependent_Propagator_Test, is_changed_only_affacts_related)
	{
		m_up_stream.touch<my_data::A>();
		EXPECT_FALSE(m_up_stream.is_changed<my_data::C>());
	}

	TEST_F(Dependent_Propagator_Test, var_name_dont_even_have_to_have_deps)
	{
		m_up_stream.touch<my_data::D>();
		EXPECT_TRUE(m_up_stream.is_changed<my_data::E>());
	}

	TEST_F(Dependent_Propagator_Test, down_stream_is_affacted)
	{
		m_up_stream.touch<my_data::A>();

		using  down_stream_list = extract_member_type_list_t<down_stream_datas>;
		Down_Stream_Datas<down_stream_list> down_stream(m_up_stream.get_all_change_status());
		EXPECT_TRUE(down_stream.is_changed<down_stream_datas::A>());
		EXPECT_TRUE(down_stream.is_changed<down_stream_datas::B>());
	}

	



}