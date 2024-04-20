
#include "gmock/gmock.h"

#include <thread>
#include <chrono>


import parallel_lib;



using namespace testing;


TEST(thread_pool_test, job_is_actually_done)
{

	bool is_executed = 0;

	{
		parallel::thread_pool l_thread_pool(4);
		l_thread_pool.add_task([&]()
			{
				is_executed = true;
			}
		);
	}

	EXPECT_TRUE(is_executed);

}

TEST(thread_pool_test, get_result_with_wait_until_job_done)
{

	using namespace std::chrono_literals;
	bool is_executed = 0;

	parallel::thread_pool l_thread_pool(4);
	l_thread_pool.add_task([&]()
		{
			std::this_thread::sleep_for(10ms);
			is_executed = true;
		}
	);
	l_thread_pool.wait_until_completed();

	EXPECT_TRUE(is_executed);

}


TEST(thread_pool_test, runinng_time_should_less)
{

	using namespace std::chrono_literals;

	std::function<void()> some_work = []()
		{
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

TEST(thread_pool_test, multiple_job_will_be_done)
{

	std::vector<bool > is_executed(4,false);

	{
		parallel::thread_pool l_thread_pool(2);

		for (int i = 0; i < is_executed.size(); i++)
		{
			l_thread_pool.add_task([&, i]() { is_executed[i] = true; });
		}

	}

	std::vector<bool > exp(4,true);

	EXPECT_THAT(is_executed, Eq(exp));

}


