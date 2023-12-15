
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

	namespace _1
	{

		TEST(Literal_Value_Test, assign_int)
		{
			constexpr auto value = literal_t< 3 >::value;
			EXPECT_THAT(value, Eq(3));
		}

		TEST(Literal_Value_Test, assign_int_2)
		{
			using int2 = std::array<int, 2>;
			constexpr auto value = literal_t< int2{1,2} >::value;
			EXPECT_THAT(value, Eq(int2{1,2}));
		}

		TEST(Literal_Value_Test, assign_enum)
		{
			constexpr auto value = literal_t<my_enum::A>::value;
			EXPECT_THAT(value, Eq(my_enum::A));
		}



		TEST(Literal_Value_Test, assign_string)
		{
			constexpr auto value = literal_t<"ABC">::value;
			std::string s = value;
			EXPECT_THAT(s, Eq("ABC"));

		}

	}

	namespace _2
	{
		TEST(Literal_Value_Test, assign_int)
		{
			constexpr auto value = literal{ 3 };
			EXPECT_THAT(value.get(), Eq(3));
		}

		TEST(Literal_Value_Test, assign_int_2)
		{
			using int2 = std::array<int, 2>;
			constexpr auto value = literal{ int2{ 1,2 } };
			EXPECT_THAT(value.get(), Eq(int2{1,2}));
		}

		TEST(Literal_Value_Test, assign_enum)
		{
			constexpr auto value = literal{my_enum::A};
			EXPECT_THAT(value.get(), Eq(my_enum::A));
		}



		TEST(Literal_Value_Test, assign_string)
		{
			constexpr auto value = literal{"ABC"};
			std::string s = value.get();
			EXPECT_THAT(s, Eq("ABC"));

		}

	}

}



