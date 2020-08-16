



#include "gmock/gmock.h"

#include "soft_render.h"




using namespace testing;
using namespace soft_render;

TEST(Vector, access)
{
	vec<3, float> a;
	a[0] = 3.f;
	EXPECT_THAT(a[0], Eq(3.f));
}

TEST(Vector, uniform_fill)
{

	vec<3, float> a = uniform_fill<3, float>(1.f);
	EXPECT_THAT(a[1], Eq(1.f));

}

TEST(Vector, add)
{
	vec<3, float> a = uniform_fill<3, float>(1.f);
	vec<3, float> b = uniform_fill<3, float>(1.f);
	vec<3, float> exp = uniform_fill<3, float>(2.f);
	auto act = a + b;
	EXPECT_THAT(act, Eq(exp));
}


TEST(Vector, draw)
{
	PipeLine_Renderer sr;
	
	sr.draw_triangle();

	//TODO
//	auto act = sr.get_result();
//	EXPECT_THAT(act[0], Ne(0));
}

TEST(Matrix, access)
{

	mat3x3f A;
	A[0][0] = 3.0f;

	EXPECT_THAT(A[0][0], Eq(3.f));
}

TEST(Matrix, init_list)
{

	mat<2, 3,int> A{ {0,1,} ,{2,3},{4,5} };

	EXPECT_THAT(A[1][1], Eq(3));
}

TEST(Matrix, get_identity)
{
	auto A = get_identity<3, 3, float>();

	EXPECT_EQ(A[0][0], 1.f);
	EXPECT_THAT(A[0][1], Eq(0.f));
}

TEST(Matrix, add)
{
	mat<3,3, float> a = uniform_fill<3,3, float>(1.f);
	mat<3,3, float> b = uniform_fill<3,3, float>(1.f);
	mat<3,3, float> exp = uniform_fill<3,3, float>(2.f);
	auto act = a + b;
	EXPECT_THAT(act, Eq(exp));
}

TEST(Matrix, solve)
{
	mat3x3f A = get_identity<3, 3, float>();

	vec3f b{ 0,1,0 };
	vec3f exp{ 0,1,0 };

	auto act = solve(A, b);

	EXPECT_THAT(act, Eq(exp));
}
