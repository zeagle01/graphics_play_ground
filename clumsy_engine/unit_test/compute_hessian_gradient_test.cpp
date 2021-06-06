
#include "gmock/gmock.h"
#include "compute_hessian_gradient.h"
#include "matrix_math/matrix_math.h"

using namespace testing;



TEST(Compute_Hessian_Gradient_Test,_1by1_case)
{
	std::vector<mat3x3f> act_A;
	std::vector<vec3f> act_b;

	vec2f weight{ 1,-1 };

	vec3f constraint_normal{ 0,0,1 };
	float violation_distance = 1e-3f;
	float stiff = 1e1f;

	Compute_Hessian_Gradient::apply(act_A, act_b, weight, constraint_normal, violation_distance, stiff);

	std::vector<mat3x3f> exp_A{
		{
			{0,0,0},
			{0,0,0},
			{0,0,stiff},
		},
		{
			{0,0,0},
			{0,0,0},
			{0,0,-stiff},
		},
		{
			{0,0,0},
			{0,0,0},
			{0,0,-stiff},
		},
		{
			{0,0,0},
			{0,0,0},
			{0,0,stiff}
		}
	};
	std::vector<vec3f> exp_b = {
		{0,0,1e-2f},
		{0,0,1e-2f}
	};

	EXPECT_TRUE(is_near_list<0>(act_A.data(), exp_A.data(), exp_A.size(), 1e-5f));
	EXPECT_TRUE(is_near_list<0>(act_b.data(), exp_b.data(), exp_b.size(), 1e-5f));
}



