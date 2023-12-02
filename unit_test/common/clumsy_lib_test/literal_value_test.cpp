
#include "gmock/gmock.h"
#include <vector>


import clumsy_lib;


using namespace testing;
using namespace clumsy_lib;



TEST(Literal_Value_Test,assign)
{
	int a;
	literal_value<3> ::assign(a);
	EXPECT_THAT(a, Eq(3));
}

