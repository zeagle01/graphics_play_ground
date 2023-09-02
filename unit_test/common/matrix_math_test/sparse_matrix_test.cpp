
#include "gmock/gmock.h"

#include <vector>


import matrix_math;


using namespace testing;

namespace sparse_matrix_test
{

	class sparse_matrix_test : public Test
	{
	protected:
		void SetUp() override
		{


		//coo
			m_row =		{ 0,1,2,0,1 };
			m_col =		{ 0,1,2,1,0 };
			m_value =	{ 1,1,1,1,1 };
		}

		//coo
		std::vector<int> m_row;
		std::vector<int> m_col;
		std::vector<float> m_value;

		sparse_matrix<float> m_sparse_m;
	};


	TEST_F(sparse_matrix_test, build)
	{
		m_sparse_m.reserve_space(m_row.size(),
				[&](int i)
				{
					return sparse_matrix<float>::r_c{ m_row[i],m_col[i] };
				}
			);

		int stencil_num = 3;


		// one vert stencil
		//	[1			]
		//	[	1		]
		//	[		1	]
		auto write_vert = m_sparse_m.get_write_loop(stencil_num,
			[](int i)
			{
				return std::vector<int>{ i };
			}
		);


		float v = 1.f;
		auto get_vert_m = [&](int si)
			{
				return [&](int i, int j) {return v; };
			};

		write_vert(get_vert_m);

		// two vert stencil
		//	[2	1		]
		//	[1	2		]
		//	[		0	]
		std::vector<int> edges{ 0,1 };
		auto write_stencils = m_sparse_m.get_write_loop(edges.size() / 2,
			[&](int i)
			{
				return std::vector<int>{edges[i * 2 + 0], edges[i * 2 + 1]};
			}
		);


		float sub_m[] = { 2.f,1.f,1.f,2.f };
		auto get_stencil_m = [&](int si)
			{
				return [&](int i, int j) {return sub_m[i * 2 + j]; };
			};

		write_stencils(get_stencil_m);

		// sum
		float sum = 0;
		auto sum_a_row = [&](int row, int col, const float& v)
			{
				sum += v;
			};
		m_sparse_m.for_each_nz(sum_a_row);

		EXPECT_THAT(sum, Eq(9.f));

	}

}
