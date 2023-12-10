
#include "gmock/gmock.h"
#include <vector>
#include <array>


import clumsy_lib;


using namespace testing;
using namespace clumsy_lib;

namespace literal_value_test
{


	enum class my_enum
	{
		A,
		B,
		C
	};

	TEST(Literal_Value_Test, assign_int)
	{
		int value = literal_value<3> ::value;
		EXPECT_THAT(value, Eq(3));
	}

	TEST(Literal_Value_Test, assign_int_2)
	{
		using int2 = std::array<int, 2>;
		auto value = literal_custom < int2, int2{ 1,2 } > ::value;
		EXPECT_THAT(value, Eq(int2{ 1,2 }));
	}

	TEST(Literal_Value_Test, assign_enum)
	{
		my_enum value = literal_value<my_enum::A> ::value;
		EXPECT_THAT(value, Eq(my_enum::A));
	}



	TEST(Literal_Value_Test, assign_string)
	{
		std::string value = literal_string<"ABC"> ::value.data();
		EXPECT_THAT(value, Eq("ABC"));

	}

}



