
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

		//csr
			m_row_cols_csr = {
				{0,1},
				{0,1,2},
				{1,2}
			};

			m_value_csr = {
				{2,1},
				{1,2,1},
					{1,2}
			};

		//coo
			m_row = { 0,1,2 };
			m_col = { 0,1,2 };
			m_value = { 1,1,1 };
		}

		//csr
		std::vector<std::vector<int> > m_row_cols_csr;
		std::vector<std::vector<float> > m_value_csr;

		//coo
		std::vector<int> m_row;
		std::vector<int> m_col;
		std::vector<float> m_value;

		sparse_matrix<float> m_sparse_m;
	};

	TEST_F(sparse_matrix_test, set_values_coo)
	{
		m_sparse_m.set_values(
			[&](int i) {

				return sparse_matrix<float>::r_c_v{
					.value = m_value[i],
					.row = m_row[i],
					.col = m_col[i],
				};
			},
			m_row.size()
		);

		float sum = 0;
		auto sum_a_row = [&](int row, int col, const float& v)
			{
				sum += v;
			};
		m_sparse_m.for_each_nz(sum_a_row);

		EXPECT_THAT(sum, Eq(3.f));
	}

	TEST_F(sparse_matrix_test, set_values_csr)
	{
		m_sparse_m.set_values(
			m_row_cols_csr.size(),
			[&](int i) {
				return m_row_cols_csr[i].size();
			},
			[&](int row,int j) {
				return sparse_matrix<float>::c_v{
					.value = m_value_csr[row][j],
					.col = m_row_cols_csr[row][j],
				};
			}
		);

		float sum = 0;
		auto sum_a_row = [&](int row, int col, const float& v)
			{
				sum += v;
			};
		m_sparse_m.for_each_nz(sum_a_row);

		EXPECT_THAT(sum, Eq(10.f));
	}

	TEST_F(sparse_matrix_test, build)
	{
		m_sparse_m.reserve_space(m_row.size(),
				[&](int i)
				{
					return sparse_matrix<float>::r_c{ m_row[i],m_col[i] };
				}
			);

		int stencil_num = 3;


		auto looper = m_sparse_m.get_write_loop(stencil_num, [](int i)
			{
				return std::vector<int>{ i };
			}
		);


		float v = 1.f;
		auto writer = [&](int i)
			{
				return [&](int i, int j) {return v; };
			};

		looper(writer);

		float sum = 0;
		auto sum_a_row = [&](int row, int col, const float& v)
			{
				sum += v;
			};
		m_sparse_m.for_each_nz(sum_a_row);

		EXPECT_THAT(sum, Eq(3.f));

	}

}
