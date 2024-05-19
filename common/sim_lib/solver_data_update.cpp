
module;

#include <vector>

module sim_lib:solver_data_update;

import matrix_math;

namespace sim_lib
{
	namespace solver_data_update
	{
		struct assign
		{
			template<typename T, int N>
			static void apply(std::vector<matrix_math::matrix<T,N, 1>>& out, const std::vector<std::array<T, N>>& in)
			{
				out.resize(in.size());
				for (int i = 0; i < in.size(); i++)
				{
					for (int j = 0; j < N; j++)
					{
						out[i](j) = in[i][j];
					}
				}

			}

			template<typename T>
			static void apply(T& out, const T& in)
			{
				out = in;
			}
		};

	}

}
