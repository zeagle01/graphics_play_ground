
module;

#include <vector>

#include "matrix_math/matrix_math.h"

module sim_lib:solver_data_update;

namespace sim_lib
{
	namespace solver_data_update
	{
		struct assign
		{
			template<typename T, int N>
			static void apply(std::vector<matrix_math::mat<N, 1, T>>& out, const std::vector<std::array<T, N>>& in)
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
