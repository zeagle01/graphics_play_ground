
#include "matrix_math/linear_system_solver.h"
#include "matrix_math/matrix_math.h"
#include "gmock/gmock.h"

using namespace testing;

using namespace matrix_math;

using mat3f = mat3x3f;

namespace linear_system_solver_test
{


	struct Linear_System
	{
		std::vector<mat3f> A;
		std::vector<int> I;
		std::vector<int> J;
	};


	Linear_System build_laplace_system(int n)
	{
		Linear_System ret;

		std::vector<std::vector<mat3f>> v(n);
		std::vector<std::vector<int>> rows(n);
		auto identity3 = get_identity<3, float>();

		for (int i = 0; i < n ; i++)
		{
			v[i].push_back(mat3f{});
			rows[i].push_back(i);
		}

		v[0][0] += identity3;
		v[n - 1][0] += identity3;

		for (int i = 0; i < n - 1; i++)
		{
			//rows[i][0].push_back(i );
			v[i][0] += identity3;

			//rows[i+1][0].push_back(i +1);
			v[i + 1][0] += identity3;

			v[i].push_back(-1.f * identity3);
			rows[i].push_back(i + 1);

			v[i + 1].push_back(-1.f * identity3);
			rows[i + 1].push_back(i);
		}

		ret.I.resize(n + 1);
		ret.I[0] = 0;
		for (int i = 0; i < v.size(); i++)
		{
			ret.I[i + 1] = v[i].size();
			for (int j = 0; j < v[i].size(); j++)
			{
				ret.A.push_back(v[i][j]);
				ret.J.push_back(rows[i][j]);
			}
		}

		for (int i = 0; i < n; i++)
		{
			ret.I[i + 1] += ret.I[i];
		}

		return ret;
	}

//	TEST(Linear_System_Solver_Test, build_laplace_system_is_right)
//	{
//		auto system = build_laplace_system(2);
//		auto identity3 = get_identity<3, float>();
//		Linear_System exp
//		{
//			.A = { 2.f * identity3,-1.f * identity3, 2.f * identity3,-1.f * identity3,},
//			.I={0,2,4},
//			.J = {0,1,1,0}
//		};
//
//		EXPECT_THAT(system.A, Eq(exp.A));
//		EXPECT_THAT(system.I, Eq(exp.I));
//		EXPECT_THAT(system.J, Eq(exp.J));
//	}
//
//	TEST(Linear_System_Solver_Test, solver_2by2)
//	{
//
//		int n = 2;
//		auto system = build_laplace_system(n);
//
//		std::vector<vec3f> b(n);
//
//		Linear_System_Solver lss;
//
//		auto x = lss.solve(system.A, system.I, system.J, b);
//		std::vector<vec3f> exp(n);
//
//		EXPECT_THAT(x, Eq(exp));
//
//	}

}
