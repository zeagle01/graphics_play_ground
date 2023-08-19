

#include "gmock/gmock.h"
#include <string>
#include "clumsy_lib/morphysm.h"
#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/class_reflection.h"

using namespace testing;
using namespace clumsy_lib;

namespace special_type_map_test
{

	struct Base
	{
		virtual std::string func() = 0;
	};

	struct A : Base
	{
		std::string func()
		{
			return "A";
		}
	};

	struct B :Base
	{
		std::string func()
		{
			return "B";
		}
	};


	struct Check_Box
	{
		CE_ADD_FIELD(A, bool);
		CE_ADD_FIELD(B, bool);
		CE_ADD_FIELD(C, bool);
	};
	using check_boxes = extract_member_type_list_t<Check_Box>;

	struct My_Var_List
	{
		CE_ADD_FIELD(A, int);
		CE_ADD_FIELD(M, Morphysm<Base>);
		CE_ADD_FIELD(Options, Static_Type_Map<check_boxes>);
	};



	using tl = extract_member_type_list_t<My_Var_List>;
	TEST(Special_Type_Map_Test, a_type_map_has_morphysm)
	{
		Static_Type_Map<tl> m;
		
		auto& obj = m.get_ref<My_Var_List::M>();
		obj.register_sub_type<A>();
		obj.register_sub_type<B>();

		{
			obj.shift<A>();
			std::string act = obj->func();
			std::string exp = "A";
			EXPECT_THAT(act, Eq(exp));
		}

		{
			obj.shift<B>();
			std::string act = obj->func();
			std::string exp = "B";
			EXPECT_THAT(act, Eq(exp));
		}
	}

	TEST(Special_Type_Map_Test, nest_type_map)
	{
		Static_Type_Map<tl> m;
		
		auto& obj = m.get_ref<My_Var_List::Options>();
		bool exp = false;
		obj.get_ref<Check_Box::A>() = exp;
		bool act = obj.get_ref<Check_Box::A>();

		EXPECT_THAT(act, Eq(exp));
	}

}
