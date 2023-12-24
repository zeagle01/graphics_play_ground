
#include "gmock/gmock.h"

#include <vector>


import matrix_math;


using namespace testing;
using namespace matrix_math;

namespace sparse_matrix_test
{
	class linear_system_test : public Test
	{
	protected:
		void SetUp() override
		{
		}

	};

	template<typename T>
	struct triplet
	{
		int col;
		int row;
		T v;
		friend static auto operator <=> (const triplet & l, const triplet & r) = default;
	};


	TEST_F(linear_system_test, assemble_diag)
	{
		linear_system<float, float>  ls;
		ls.set_variables_num(2);

		auto diag_loop = ls.get_write_loop(2,
			[](int i) {return  std::vector<int>{ i }; }
		);

		auto wirte_diag = [](auto lhs, auto rhs, int si)
			{
				lhs( 0, 0) = 1.f;
				rhs( 0) = 1.f;
			};

		diag_loop(wirte_diag);

		std::vector<triplet<float>> exp{ {0,0,1.f},{1,1,1.f} };
		std::vector<triplet<float>> act;

		ls.for_each_nz(
			[&act](int row, int col, const float& v)
			{
				act.push_back({ row,col,v });
			}
		);

		EXPECT_THAT(act,Eq(exp));
	}

	TEST_F(linear_system_test, assemble_interaction_edges)
	{
		linear_system<float, float>  ls;
		ls.set_variables_num(2);

		std::vector<int> edges{ 0,1 };
		auto diag_loop = ls.get_write_loop(edges.size() / 2,
			[&](int i) {return  std::vector<int>{ edges[i * 2 + 0], edges[i * 2 + 1] }; }
		);

		auto wirte_diag = [](auto lhs, auto rhs, int si)
			{
				lhs( 0, 0) = 1.f;
				lhs( 0, 1) = -1.f;
				lhs( 1, 0) = -1.f;
				lhs( 1, 1) = 1.f;
				rhs( 0) = 1.f;
			};

		diag_loop(wirte_diag);

		std::vector<triplet<float>> exp{ 
			{0,0,1.f},
			{0,1,-1.f},
			{1,0,-1.f},
			{1,1,1.f}
		};
		std::vector<triplet<float>> act;

		ls.for_each_nz(
			[&act](int row, int col, const float& v)
			{
				act.push_back({ row,col,v });
			}
		);

		EXPECT_THAT(act,Eq(exp));
	}

	TEST_F(linear_system_test, solve_with_Jacobi)
	{
		linear_system<float, float>  ls;
		int vert_num = 2;
		ls.set_variables_num(vert_num);

		std::vector<int> edges{ 0,1 };
		auto diag_loop = ls.get_write_loop(edges.size() / 2,
			[&](int i) {return  std::vector<int>{ edges[i * 2 + 0], edges[i * 2 + 1] }; }
		);

		auto wirte_diag = [](auto lhs, auto rhs, int si)
			{
				lhs( 0, 0) = 2.f;
				lhs( 0, 1) = -1.f;
				lhs( 1, 0) = -1.f;
				lhs( 1, 1) = 2.f;

				rhs( 0) = 1.f;
				rhs( 1) = 1.f;
			};

		diag_loop(wirte_diag);

		Jacobi_solver<float, float> solver;
		std::vector<float> x(vert_num);
		solver.solve(ls, x);

		std::vector<float> exp{ 1.f,1.f };
		EXPECT_THAT(x, Eq(exp));

	}

}
