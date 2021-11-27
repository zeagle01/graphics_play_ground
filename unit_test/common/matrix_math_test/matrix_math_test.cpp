#include "matrix_math/matrix_math.h"
#include "gmock/gmock.h"

using namespace testing;

namespace matrix_math
{
	TEST(Matrix_Math_Test, init)
	{
		mat< 2, 1, float> act{ 1,2 };

		EXPECT_THAT(act(0, 0), Eq(1));
		EXPECT_THAT(act(1, 0), Eq(2));

		EXPECT_THAT(act(0), Eq(1));
		EXPECT_THAT(act(1), Eq(2));
	}

	TEST(Matrix_Math_Test, add_minus)
	{
		mat< 2, 1, float> a{ 1,2 };
		mat< 2, 1, float> b{ 1,2 };

		auto act = a + b;

		EXPECT_THAT(act(0), Eq(2));
		EXPECT_THAT(act(1), Eq(4));

		act = a - b;

		EXPECT_THAT(act(0), Eq(0));
		EXPECT_THAT(act(1), Eq(0));
	}

	TEST(Matrix_Math_Test, self_add)
	{
		mat< 2, 1, float> a{ 1,2 };
		mat< 2, 1, float> b{ 1,2 };

		auto act = a ;

		act += b;
		EXPECT_THAT(act(0), Eq(2));
		EXPECT_THAT(act(1), Eq(4));

		act -=  b;
		EXPECT_THAT(act(0), Eq(1));
		EXPECT_THAT(act(1), Eq(2));
	}

	TEST(Matrix_Math_Test, matrix_multiply_scalar)
	{
		mat< 2, 1, float> a{ 1,2 };

		auto act = a * 2.f;

		EXPECT_THAT(act(0), Eq(2));
		EXPECT_THAT(act(1), Eq(4));

		act = 3.f * a;

		EXPECT_THAT(act(0), Eq(3));
		EXPECT_THAT(act(1), Eq(6));

		act = act / 3.f;

		EXPECT_THAT(act(0), Eq(1));
		EXPECT_THAT(act(1), Eq(2));
	}

	TEST(Matrix_Math_Test, matrix_multiply)
	{
		mat< 2, 1, float> a{ 1,2 };
		mat< 1, 2, float> b{ 1,2 };

		auto act = a * b;

		EXPECT_THAT(act(0,0), Eq(1));
		EXPECT_THAT(act(1,0), Eq(2));
		EXPECT_THAT(act(0,1), Eq(2));
		EXPECT_THAT(act(1,1), Eq(4));

		auto act1 = b * a;

		EXPECT_THAT(act1, Eq(5));
	}

	TEST(Matrix_Math_Test, cross)
	{
		vec3f v0{ 1,0,0 };
		vec3f v1{ 0,1,0 };

		auto exp = v0 ^ v1;
		EXPECT_THAT(exp(0), Eq(0));
		EXPECT_THAT(exp(1), Eq(0));
		EXPECT_THAT(exp(2), Eq(1));
	}

	TEST(Matrix_Math_Test, solve)
	{
		mat3x3f A = get_identity<3, float>();

		vec3f b{ 0,1,0 };
		vec3f exp{ 0,1,0 };

		auto act = b / A;

		EXPECT_THAT(act(0), Eq(exp(0)));
		EXPECT_THAT(act(1), Eq(exp(1)));
		EXPECT_THAT(act(2), Eq(exp(2)));
	}

	TEST(Matrix_Math_Test, vectorize_test)
	{
		auto A = get_identity<2, float>();


		auto act = vectorize(A);
	}

	TEST(Matrix_Math_Test, norm_test)
	{
		auto A = get_identity<2, float>();

		auto act = norm<2>::apply(A);

		EXPECT_THAT(act, Eq(sqrt(2.f)));
	}

	TEST(Matrix_Math_Test, is_near_test)
	{
		vec3f v0{ 1,0,0 };
		vec3f v1{ 1,0,0 };


		EXPECT_TRUE(is_near<2>(v0, v1));

		 v0(0)+=0.0001;

		EXPECT_FALSE(is_near<2>(v0, v1));
	}
}
