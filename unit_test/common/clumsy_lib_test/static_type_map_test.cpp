
#include "gmock/gmock.h"

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/class_reflection.h"

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
