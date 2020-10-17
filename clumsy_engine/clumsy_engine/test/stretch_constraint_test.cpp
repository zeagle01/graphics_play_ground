


#include "gmock/gmock.h"

using namespace testing;




class Edge_Stretch
{
public:
	std::vector<float> update(const std::vector<float>& x)
	{
		return std::vector<float>();
	}

};






TEST(Stretch_Constraint_Test, shrink_to_length)
{

	Edge_Stretch es;

	std::vector<float> x
	{

	};

	auto act = es.update(x);

	std::vector<float> exp
	{

	};

	EXPECT_THAT(act, Eq(exp));

}

