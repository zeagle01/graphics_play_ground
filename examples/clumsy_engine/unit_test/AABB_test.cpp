
#include "gmock/gmock.h"
#include "AABB.h"

using namespace testing;

TEST(AABB_Test,two_points)
{
	vec2f lower{ 0,0 };
	vec2f upper{ 1,1 };

	AABB<2> aabb;
	aabb += lower;
	aabb += upper;

	is_near<0>(aabb.m_lower, lower);
	is_near<0>(aabb.m_upper, upper);

}