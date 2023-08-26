

#include "clumsy_lib/get_instance.h"


#include "classes_for_get_instance.h"

#include "gmock/gmock.h"

#include <memory>

using namespace testing;
using namespace clumsy_lib;




using namespace get_instance_test;



namespace get_instance_test
{
	TEST(Get_Derived_Instance_By_Enum_Test, get_new_instance)
	{
		auto obj = creator<my_interface>::apply<data_type::A>();
		EXPECT_THAT(obj->bark(), Eq("A"));
	}

	TEST(Get_Derived_Instance_By_Enum_Test, get_new_instance_by_dynamic_enum)
	{
		auto obj = get_instance::apply<my_interface>(data_type::A);
		EXPECT_THAT(obj->bark(), Eq("A"));
	}

}
