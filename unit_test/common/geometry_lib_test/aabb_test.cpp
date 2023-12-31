

#include "matrix_math/matrix_math.h"
#include "gmock/gmock.h"

import geometry_lib;
import matrix_math;

using namespace testing;


namespace aabb_test
{

	TEST(aabb_test, default_aabb_not_intersect)
	{

		geometry::AABB1f b;
		geometry::AABB1f a;

		EXPECT_FALSE(a.intersect(b));
	}

	TEST(aabb_test, point_not_intersec_with_default_aabb)
	{

		geometry::AABB1f b;
		geometry::AABB1f a;
		a += 0.f;

		EXPECT_FALSE(a.intersect(b));
	}

	TEST(aabb_test, aabb_intersec_with_default_aabb)
	{

		geometry::AABB1f b;
		geometry::AABB1f a;
		a += 0.f;
		a += 1.f;

		EXPECT_FALSE(a.intersect(b));
	}

	TEST(aabb_test, aabb_not_intersect)
	{

		geometry::AABB1f b;
		b += -1.f;
		b += -2.f;
		geometry::AABB1f a;
		a += 0.f;
		a += 1.f;

		EXPECT_FALSE(a.intersect(b));
	}

	TEST(aabb_test, aabb_intersect)
	{

		geometry::AABB1f b;
		b += 0.2f;
		b += 0.5f;
		geometry::AABB1f a;
		a += 0.f;
		a += 1.f;

		EXPECT_TRUE(a.intersect(b));
	}

	template<typename T,int N>
	using my_vec = matrix_math::mat<N, 1, T>;

	TEST(aabb_test, aabb_intersect_AABB2f)
	{

		geometry::AABB2f<my_vec> b;
		b += {0.2f, 0.3f};
		b += {0.5f, 0.8f};
		geometry::AABB2f<my_vec> a;
		a += {0.f, 0.f};
		a += {1.f, 1.f};

		EXPECT_TRUE(a.intersect(b));
	}

}
