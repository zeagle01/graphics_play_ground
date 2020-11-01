

#include "gmock/gmock.h"

#include "simulation_data.h"
#include <functional>



using namespace testing;
using namespace clumsy_engine;


struct observer
{
	void process(int a)
	{

		data = a;
	}

	int data;
};

TEST(Signal_Test, set_Source_Datadata)
{
	signal<const int&> d;

	observer ob;

	auto s = std::bind(&observer::process, &ob, std::placeholders::_1);

	d.connect(s);

	int exp = 10;

	d(exp);


	EXPECT_THAT(ob.data, Eq(exp));

}

