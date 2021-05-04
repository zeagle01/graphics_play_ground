
#include "gmock/gmock.h"
#include "clumsy_lib/macro_loop.h"

using namespace testing;

#define PUSH_BACK_LIST(i,v) v.push_back(i);

TEST(Macro_Test, single_loop_with_index)
{
	std::vector<int > v;
	LOOP(3, PUSH_BACK_LIST, PUSH_BACK_LIST, v);


	std::vector<int > exp{ 0,1,2 };
	EXPECT_THAT(v, Eq(exp));

}



#define PUSH_BACK_LIST_OF_LIST(i,v) INSIDE_LOOP(i,PUSH_BACK_LIST,PUSH_BACK_LIST,v)

TEST(Macro_Test, double_loop_with_index)
{
	std::vector<int> v;
	EVAL(LOOP(4, PUSH_BACK_LIST_OF_LIST, PUSH_BACK_LIST_OF_LIST, v));


	std::vector<int> exp
	{
		0,
		0,1 ,
		0,1 ,2
	};

	EXPECT_THAT(v, Eq(exp));

}

TEST(Macro_Test, single_loop_32)
{
	std::vector<int > v;
	LOOP(32, PUSH_BACK_LIST, PUSH_BACK_LIST, v);

	std::vector<int> exp;
	for (int i = 0; i < 32; i++)
	{
		exp.push_back(i);
	}

	EXPECT_THAT(v, Eq(exp));

}
