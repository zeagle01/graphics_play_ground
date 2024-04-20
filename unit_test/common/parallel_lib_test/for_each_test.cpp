

#include "gmock/gmock.h"

import parallel_lib;


using namespace testing;


TEST(for_each_test, job_is_actually_done)
{
	bool is_executed = 0;

	parallel::for_each(1, 1, [&](int i) {is_executed = true; });

	EXPECT_TRUE(is_executed);

}

TEST(for_each_test, vectorized_compuation_compoent_check)
{
	int n = 100;
	std::vector<int> a(n, 1);
	std::vector<int> b(n, 1);
	std::vector<int> c(n);

	parallel::for_each(n, 64, [&](int i) {c[i] = a[i] + b[i]; });

	for (int i = 0; i < n; i++)
	{
		EXPECT_THAT(c[i], Eq(2));
	}
}

TEST(for_each_test, two_for_each_in_a_row)
{
	int n = 100;
	std::vector<int> a(n, 1);
	std::vector<int> b(n, 1);
	std::vector<int> c(n);

	parallel::for_each(n, 64, [&](int i) {c[i] = a[i] + b[i]; });
	parallel::for_each(n, 64, [&](int i) {c[i] *= 2; });

	for (int i = 0; i < n; i++)
	{
		EXPECT_THAT(c[i], Eq(4));
	}
}
