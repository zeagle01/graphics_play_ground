
#include "gmock/gmock.h"

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/static_loop.h"

using namespace testing;
using namespace clumsy_lib;


struct My_List
{
	ADD_FIELD(A, int);
	ADD_FIELD(B, float);
};

using tl = extract_member_type_list_t<My_List>;

TEST(Static_Type_Map_Test, get_ref)
{
	clumsy_lib::Static_Type_Map<tl> m;


	int exp = 2;
	m.get_ref<My_List::A>() = exp;

	int act = m.get_ref<My_List::A>();

	EXPECT_THAT(act, Eq(exp));
}


TEST(Static_Type_Map_Test, get_B)
{
	clumsy_lib::Static_Type_Map<tl> m;

	float exp = 2.f;
	m.get_ref<My_List::B>() = exp;

	float act = m.get_ref<My_List::B>();

	EXPECT_THAT(act, Eq(exp));
}


/////////////////////////////////////////

template<typename T,T v>
struct LiteralValue
{
	static constexpr auto apply()
	{
		return v;
	}
};


struct Var_With_Defualt_Value
{
	ADDD_FIELD_WITH(A, int, ADD_FIELD(Get_Default, EVAL(LiteralValue<int,2>)))
	ADDD_FIELD_WITH(B, float, ADD_FIELD(Get_Default, EVAL(LiteralValue<float,3.f>)))

};

struct Set_Defual_Value
{
	template<typename Var_Name,typename Type_Map>
	static void apply(Type_Map& tm)
	{
		tm.get_ref<Var_Name>() = Var_Name::Get_Default::type::apply();
	}

};


TEST(Static_Type_Map_Test, set_defualt_value)
{
	using default_value_vars = extract_member_type_list_t<Var_With_Defualt_Value>;
	clumsy_lib::Static_Type_Map<default_value_vars> m;

	For_Each_Type<default_value_vars>::apply<Set_Defual_Value>(m);

	int actA = m.get_ref<Var_With_Defualt_Value::A>();
	EXPECT_THAT(actA, Eq(2));

	float actB = m.get_ref<Var_With_Defualt_Value::B>();
	EXPECT_THAT(actB, Eq(3.f));
}
