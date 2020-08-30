



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
	//PipeLine_Renderer sr;
	
	//sr.draw_triangle();

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

TEST(Matrix, init_with_vecs)
{

	vec2f a{ 0,1 };
	vec2f b{ 2,3 };
	mat<2, 2, float> act{ {a,b} };

	mat<2, 2, float> exp{ {0,1},{2,3} };

	EXPECT_THAT(act, Eq(exp));
}

TEST(Matrix, get_identity)
{
	auto act = get_identity<3, 3, float>();

	mat3x3f exp{ {1,0,0},{0,1,0},{0,0,1} };

	EXPECT_THAT(act, Eq(exp));
}

TEST(Matrix, add)
{
	mat<3,3, float> a = uniform_fill<3,3, float>(1.f);
	mat<3,3, float> b = uniform_fill<3,3, float>(1.f);
	mat<3,3, float> exp = uniform_fill<3,3, float>(2.f);
	auto act = a + b;
	EXPECT_THAT(act, Eq(exp));
}

TEST(Matrix, times_matrix)
{
	mat<3,3, float> a = uniform_fill<3,3, float>(2.f);
	mat<3,3, float> b = uniform_fill<3,3, float>(2.f);
	mat<3,3, float> exp = uniform_fill<3,3, float>(4.f);
	auto act = a * b;
	EXPECT_THAT(act, Eq(exp));
}

TEST(Matrix, times_vector)
{
	mat<3,1, float> a = uniform_fill<3,1, float>(2.f);
	mat<3,1, float> b = uniform_fill<3,1, float>(2.f);
	mat<3,1, float> exp = uniform_fill<3,1, float>(4.f);
	auto act = a * b;
	EXPECT_THAT(act, Eq(exp));
}

TEST(Matrix, matrix_multiply_matrix)
{
	mat<2, 2, float> a{ {1,2},{3,4} };
	mat<2, 2, float> b = uniform_fill<2, 2, float>(1.f);
	mat<2, 2, float> exp = { {4,6} ,{4,6} };
	auto act = a % b;
	EXPECT_THAT(act, Eq(exp));

}

TEST(Matrix, matrix_multiply_vector)
{
	mat<2, 2, float> a{ {1,2},{3,4} };
	mat<2, 1, float> b = uniform_fill<2, 1, float>(1.f);
	mat<2, 1, float> exp = { 4,6 };
	auto act = a % b;
	EXPECT_THAT(act, Eq(exp));

}

TEST(Matrix, determinant_2by2)
{
	mat2x2f A{ {1,2},{3,4} };

	auto act = determinant<2, float>::get(A);

	float exp = -2.f;

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

TEST(Matrix, solve_2112)
{
	mat2x2f A{ {2,1},{1,2} };

	vec2f b{ 3,3, };
	vec2f exp{ 1,1 };

	auto act = solve(A, b);

	EXPECT_THAT(act, Eq(exp));
}


TEST(Matrix, look_at_point)
{

	vec4f p{ 0.0,0.0,0.0,1 };

	mat4x4f m = lookat_matrix<float>({ 0,0,3 }, { 0,0,0 }, { 0,1,0 });


	auto act = m % p;

	vec4f exp{ 0,0,-3 };

	EXPECT_THAT(act, Eq(exp));
}

TEST(Matrix, perspective_point)
{

	vec4f p{ 0.5,0.5,0.5,1 };

	mat4x4f m = perspective_matrix<float>(1, 1, 1, 10);


	auto act = m % p;

	vec4f exp{};

	EXPECT_THAT(act, Eq(exp));
}


