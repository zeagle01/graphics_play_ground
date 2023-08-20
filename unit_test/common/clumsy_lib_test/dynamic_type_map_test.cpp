
#include "gmock/gmock.h"
#include "clumsy_lib/dynamic_type_map.h"
#include "clumsy_lib/field_defines.h"

using namespace testing;
using namespace clumsy_lib;


struct List_A
{
	CE_ADD_FIELD(A, int);
	CE_ADD_FIELD(B, float);
};

struct List_B
{
	CE_ADD_FIELD(A, int);
	CE_ADD_FIELD(B, float);
};

TEST(Dynamic_Type_Map_Test, add_varialbe_to_type_map)
{
	Dynamic_Type_Map m;
	m.add_or_replace<List_A::A>();
	m.add_or_replace<List_A::B>();

	m.add_or_replace<List_B::A>();
	m.add_or_replace<List_B::B>();



	int exp_aa = 2;
	float exp_ab = 2.f;
	int exp_ba = 3;
	float exp_bb = 3.f;

	m.get_ref<List_A::A>() = exp_aa;
	m.get_ref<List_A::B>() = exp_ab;
	m.get_ref<List_B::A>() = exp_ba;
	m.get_ref<List_B::B>() = exp_bb;

	const int& act_aa = m.get_ref<List_A::A>();
	const float& act_ab = m.get_ref<List_A::B>();
	const int& act_ba = m.get_ref<List_B::A>();
	const float& act_bb = m.get_ref<List_B::B>();

	EXPECT_THAT(act_aa, Eq(exp_aa));
	EXPECT_THAT(act_ab, Eq(exp_ab));
	EXPECT_THAT(act_ba, Eq(exp_ba));
	EXPECT_THAT(act_bb, Eq(exp_bb));

}

TEST(Dynamic_Type_Map_Test, copy_type_map_and_const_ref)
{

	Dynamic_Type_Map m;
	m.add_or_replace<List_A::A>();
	m.add_or_replace<List_A::B>();

	m.add_or_replace<List_B::A>();
	m.add_or_replace<List_B::B>();

	int exp_aa = 2;
	float exp_ab = 2.f;
	int exp_ba = 3;
	float exp_bb = 3.f;

	m.get_ref<List_A::A>() = exp_aa;
	m.get_ref<List_A::B>() = exp_ab;
	m.get_ref<List_B::A>() = exp_ba;
	m.get_ref<List_B::B>() = exp_bb;

	const Dynamic_Type_Map m1 = m;

	const int& act_aa = m1.get_ref<List_A::A>();
	const float& act_ab = m1.get_ref<List_A::B>();
	const int& act_ba = m1.get_ref<List_B::A>();
	const float& act_bb = m1.get_ref<List_B::B>();

	EXPECT_THAT(act_aa, Eq(exp_aa));
	EXPECT_THAT(act_ab, Eq(exp_ab));
	EXPECT_THAT(act_ba, Eq(exp_ba));
	EXPECT_THAT(act_bb, Eq(exp_bb));

}

