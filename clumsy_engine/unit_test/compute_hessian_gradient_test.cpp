
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

	clumsy_engine::Compute_Hessian_Gradient::apply(act_A, act_b, weight, constraint_normal, violation_distance, stiff);

	std::vector<mat3x3f> exp_A{
		{
			0,0,0,
			0,0,0,
			0,0,stiff,
		},
		{
			0,0,0,
			0,0,0,
			0,0,-stiff,
		},
		{
			0,0,0,
			0,0,0,
			0,0,-stiff,
		},
		{
			0,0,0,
			0,0,0,
			0,0,stiff
		}
	};
	std::vector<vec3f> exp_b = {
		{0,0,1e-2f},
		{0,0,1e-2f}
	};

	for (int i = 0; i < exp_A.size(); i++)
	{
		EXPECT_TRUE(is_near<2>(act_A[i], exp_A[i]));
	}

	for (int i = 0; i < exp_b.size(); i++)
	{
		EXPECT_TRUE(is_near<2>(act_b[i], exp_b[i]));
	}
}



