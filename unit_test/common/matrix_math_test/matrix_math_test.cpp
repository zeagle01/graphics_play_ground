


#include "matrix_math/matrix_math.h"
#include "gmock/gmock.h"


using namespace testing;



TEST(Matrix_Test, acess_by_element)
{
	mat<2, 2, float> m
	{
		{0,1},
		{2,3}
	};

	EXPECT_THAT(m[0][0], Eq(0));
	EXPECT_THAT(m[0][1], Eq(1));
	EXPECT_THAT(m[1][0], Eq(2));
	EXPECT_THAT(m[1][1], Eq(3));
}

TEST(Matrix_Test, assign_by_element)
{
	mat<2, 2, float> m
	{
		{0,1},
		{2,3}
	};
	m[0][0] = 5;

	EXPECT_THAT(m[0][0], Eq(5));
}

TEST(Matrix_Test, get_column)
{
	mat<2, 2, float> m
	{
		{0,1},
		{2,3}
	};
	vec<2,float> c = m[0];

	EXPECT_THAT(c[0], Eq(0));
	EXPECT_THAT(c[1], Eq(1));
}

TEST(Vector_Test, get_element)
{
	vec<2,  float> v{0,1};

	EXPECT_THAT(v[0], Eq(0));
	EXPECT_THAT(v[1], Eq(1));
}
