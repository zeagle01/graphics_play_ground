



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

	vec<3, float> a = uniform_vec<3, float>(1.f);
	EXPECT_THAT(a[1], Eq(1.f));

}

TEST(Vector, add)
{
	vec<3, float> a = uniform_vec<3, float>(1.f);
	vec<3, float> b = uniform_vec<3, float>(1.f);
	vec<3, float> exp = uniform_vec<3, float>(2.f);
	auto act = a + b;
	EXPECT_THAT(act, Eq(exp));
}


TEST(Vector, draw)
{
	Soft_Renderer sr;
	struct vertex
	{
		vec4f position;
		vec4f color;
	};

	vertex vertices[] = {
		{{0,0,0,1},{0,1,0,1}},
		{{1,0,0,1},{1,0,0,1}},
		{{1,1,0,1},{0,0,1,1}}
	};

	sr.set_vertex_shader();
	
	sr.render();

	//TODO
//	auto act = sr.get_result();
//	EXPECT_THAT(act[0], Ne(0));
}
