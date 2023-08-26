
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

	class Dependent_Propagator_Test :public Test
	{
	protected:
		using  tl = extract_member_type_list_t<my_data>;
		Dependent_Propagator<tl> m_dep_propagator;
	};


	TEST_F(Dependent_Propagator_Test, touched_is_changed)
	{
		m_dep_propagator.touch<my_data::A>();
		EXPECT_TRUE(m_dep_propagator.is_changed<my_data::A>());
	}

	TEST_F(Dependent_Propagator_Test, is_changed_is_affacted_by_deps)
	{
		m_dep_propagator.touch<my_data::A>();
		EXPECT_TRUE(m_dep_propagator.is_changed<my_data::B>());
	}

	TEST_F(Dependent_Propagator_Test, is_changed_only_affacts_related)
	{
		m_dep_propagator.touch<my_data::A>();
		EXPECT_FALSE(m_dep_propagator.is_changed<my_data::C>());
	}

	TEST_F(Dependent_Propagator_Test, var_name_dont_even_have_to_have_deps)
	{
		m_dep_propagator.touch<my_data::D>();
		EXPECT_TRUE(m_dep_propagator.is_changed<my_data::E>());
	}

	



}