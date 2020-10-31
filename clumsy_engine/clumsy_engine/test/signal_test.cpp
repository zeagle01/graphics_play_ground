

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

TEST(Signal_Test, set_broad_cast_data)
{
	Broad_Cast_Data<int> d;
	observer ob;

	auto s = std::bind(&observer::process, &ob, std::placeholders::_1);

	d.send_to(s);

	int exp = 10;

	d.set(exp);

	EXPECT_THAT(ob.data, Eq(exp));

}

