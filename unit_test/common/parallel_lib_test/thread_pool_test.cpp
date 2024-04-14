
#include "gmock/gmock.h"

#include <thread>
#include <chrono>


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


TEST(thread_pool_test, runinng_time_should_less)
{

	using namespace std::chrono_literals;

	std::function<void()> some_work = []()
		{
			printf(" %d doing job ...\n",std::this_thread::get_id());
			std::this_thread::sleep_for(20ms);

		};


	auto begin = std::chrono::high_resolution_clock::now();

	{
		parallel::thread_pool l_thread_pool(4);

		l_thread_pool.add_task(some_work);
		l_thread_pool.add_task(some_work);
		l_thread_pool.add_task(some_work);
		l_thread_pool.add_task(some_work);

	}
	auto end = std::chrono::high_resolution_clock::now();
	auto act = (end - begin).count() * 1e-6;

	EXPECT_THAT(act, Lt(80));

}

