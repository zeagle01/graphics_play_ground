
#include "gmock/gmock.h"


import parallel_lib;



using namespace testing;


TEST(thread_pool_test, init_a_thread_pool)
{

	int act = 0;

	{
		parallel::thread_pool l_thread_pool(4);
		l_thread_pool.add_task([&]()
			{
				act++;
			}
		);
	}

	EXPECT_THAT(act, Eq(1));

}


