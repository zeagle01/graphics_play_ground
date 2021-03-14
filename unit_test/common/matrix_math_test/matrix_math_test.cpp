


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

TEST(Matrix_Test, assign_construct)
{
	mat<2, 2, float> m
	{
		{0,1},
		{2,3}
	};
	mat<2, 2, float> m1(m);

	EXPECT_THAT(m1[0][0], Eq(0));
	EXPECT_THAT(m1[0][1], Eq(1));
	EXPECT_THAT(m1[1][0], Eq(2));
	EXPECT_THAT(m1[1][1], Eq(3));
}

TEST(Vector_Test, assign_construct)
{
	vec2f v{0,1};
	auto v1 = v;

	EXPECT_THAT(v1[0], Eq(0));
	EXPECT_THAT(v1[1], Eq(1));
}




/// add
TEST(Matrix_Test, add)
{
	mat2x2f m0 = get_uniform<2, 2, float>(1);
	mat2x2f m1 = get_uniform<2, 2, float>(1);

	auto exp = m0 + m1;
	EXPECT_THAT(exp[0][0], Eq(2));
	EXPECT_THAT(exp[0][1], Eq(2));
	EXPECT_THAT(exp[1][0], Eq(2));
	EXPECT_THAT(exp[1][1], Eq(2));
}

TEST(Vector_Test, add)
{
	vec2f v0{1,1};
	vec2f v1{1,1};

	auto exp = v0 + v1;
	EXPECT_THAT(exp[0], Eq(2));
	EXPECT_THAT(exp[1], Eq(2));
}

/// norm
TEST(Vector_Test, norm)
{
	vec2f v0{1,1};
	vec2f v1{3,4};

	float norm1 = Vectorized_Norm<1>()(v0); //1+1
	float norm2 = Vectorized_Norm<2>()(v1); // sqrt(3*3+4*4)=5
	float norm3 = Vectorized_Norm<0>()(v1); //infinity norm
	float norm4 = Vectorized_Norm<1>()(v1 - v0); //2+3
	EXPECT_THAT(norm1, Eq(2));
	EXPECT_THAT(norm2, Eq(5));
	EXPECT_THAT(norm3, Eq(4));
	EXPECT_THAT(norm4, Eq(5));
}

TEST(Matrix_Test, norm)
{
	mat2x2f m0 = get_uniform<2, 2, float>(1);
	float exp = Vectorized_Norm<1>()(m0);
	EXPECT_THAT(exp, Eq(4));
}


/// is_near
TEST(Vector_Test, is_near)
{
	vec2f v0{1,1};
	vec2f v1{1,1};
	vec2f v2{1,1.1};

	auto exp1 = is_near<1>(v1, v0);
	auto exp2 = is_near<1>(v2, v0);
	auto exp3 = is_near<1>(v2, v0, 0.11f);
	EXPECT_TRUE(exp1);
	EXPECT_FALSE(exp2);
	EXPECT_TRUE(exp3);
}
