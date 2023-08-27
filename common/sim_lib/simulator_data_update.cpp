
module;

#include "matrix_math/matrix_math.h"

#include <vector>
#include <set>

module sim_lib : simulator_data_update;

import :sim_data;
import :small_mat;


namespace sim_lib
{
	namespace simulator_data_update
	{

		struct assign
		{
			template<typename T, int N>
			static void apply(std::vector<std::array<T, N>>& out, const std::vector<matrix_math::mat<N, 1, T>>& in)
			{
				out.resize(in.size());
				for (int i = 0; i < in.size(); i++)
				{
					for (int j = 0; j < N; j++)
					{
						out[i][j] = in[i](j);
					}
				}
			}
		};


		struct compute_dynamic_vert_index_map
		{
			static void apply(std::vector<int>& out, const int& vert_size, const std::vector<int>& obstacles_vertex_indices)
			{
				out.clear();
				out.reserve(vert_size);

				std::set<int> obstacles_vertex_indices_set(obstacles_vertex_indices.begin(), obstacles_vertex_indices.end());

				for (int i = 0; i < vert_size; i++)
				{
					if (!obstacles_vertex_indices_set.contains(i))
					{
						out.push_back(i);
					}
				}
				out.shrink_to_fit();

			}

		};


		struct compute_dynamic_positions
		{
			static void apply(std::vector<vec3>& out, const std::vector<float3>& in, const std::vector<int>& index_map)
			{
				out.clear();
				out.reserve(index_map.size());
				for (int i = 0; i < index_map.size(); i++)
				{
					int v = index_map[i];
					out.push_back({ in[v][0] ,in[v][1] ,in[v][2] });
				}
				out.shrink_to_fit();
			}
		};

		struct map_dynamic_positions_back_to_interface_positions
		{
			static void apply(std::vector<float3>& out, const std::vector<vec3>& in, const std::vector<int>& index_map)
			{
				for (int i = 0; i < index_map.size(); i++)
				{
					int v = index_map[i];
					out[v][0] = in[i](0); 
					out[v][1] = in[i](1); 
					out[v][2] = in[i](2); 
				}
			}

		};

	}
}

