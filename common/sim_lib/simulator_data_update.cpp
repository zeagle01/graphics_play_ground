
module;

#include "matrix_math/matrix_math.h"

#include <vector>
#include <ranges>
#include <algorithm>
#include <set>
#include <unordered_map>

module sim_lib : simulator_data_update;

import :sim_data;
import :small_mat;


namespace sim_lib
{
	namespace simulator_data_update
	{

		struct assign
		{

			template<typename T >
			static void apply(T& out, const T& in)
			{
				out = in;
			}

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

			template<typename T, int N>
			static void apply(std::array<T, N>& out, const matrix_math::mat<N, 1, T>& in)
			{
				for (int j = 0; j < N; j++)
				{
					out[j] = in(j);
				}
			}

			template<typename T, int N>
			static void apply(matrix_math::mat<N, 1, T>& out,const std::array<T, N>& in)
			{
				for (int j = 0; j < N; j++)
				{
					out(j) = in[j];
				}
			}
		};

		struct resize
		{
			template<typename T>
			static void apply(std::vector<T>& out, int n)
			{
				out.resize(n);
			}
		};

		struct compute_edge_lengths
		{
			template<typename vec>
			static void apply(std::vector<float>& out, const std::vector<int2>& edges, const std::vector<vec>& pos)
			{
				out.resize(edges.size());
				for (int i = 0; i < edges.size(); i++)
				{
					auto dx = pos[edges[i][0]] - pos[edges[i][1]];
					out[i] = matrix_math::norm<2>::apply(dx);
				}
			}

		};

		struct compute_vert_adj_verts
		{
			static void apply(std::vector<std::vector<int>>& out, const std::vector<int3>& triangles)
			{
				std::vector<std::set<int>> vv;
				vv.reserve(triangles.size() / 2);
				for (int i = 0; i < triangles.size(); i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (vv.size() <= triangles[i][j])
						{
							vv.resize(triangles[i][j] + 1);
						}
					}

					int v0 = triangles[i][0];
					int v1 = triangles[i][1];
					int v2 = triangles[i][2];

					vv[v0].insert(v1);
					vv[v1].insert(v0);
					vv[v0].insert(v2);
					vv[v2].insert(v0);
					vv[v1].insert(v2);
					vv[v2].insert(v1);
				}

				out.resize(vv.size());
				for (int i = 0; i < vv.size(); i++)
				{
					std::ranges::copy(vv[i], std::back_inserter(out[i]));

				}

			}
		};

		struct compute_interface_verts
		{
			static void apply(std::vector<int>& out, const std::vector<std::vector<int>>& vv, const std::vector<int>& obstacles_vertex_indices)
			{
				out.clear();
				out.reserve(vv.size());

				std::set<int> obstacles_vertex_indices_set(obstacles_vertex_indices.begin(), obstacles_vertex_indices.end());
				for (int i = 0; i < obstacles_vertex_indices.size(); i++)
				{
					int obstacle_v = obstacles_vertex_indices[i];
					for (int j = 0; j < vv[obstacle_v].size(); j++)
					{
						int adj_v = vv[obstacle_v][j];
						bool adj_v_is_dynamic = !obstacles_vertex_indices_set.contains(adj_v);
						if (adj_v_is_dynamic)
						{
							out.push_back(obstacle_v);
							break;
						}

					}
				}

			}
		};

		struct compute_internal_dynamic_verts
		{
			static void apply(std::vector<int>& out, const int& vert_size, const std::vector<int>& obstacles_vertex_indices)
			{
				out.clear();

				std::set<int> obstacles_vertex_indices_set(obstacles_vertex_indices.begin(), obstacles_vertex_indices.end());

				for (int i = 0; i < vert_size; i++)
				{
					if (!obstacles_vertex_indices_set.contains(i))
					{
						out.push_back(i);
					}
				}

			}

		};


		struct compute_dynamic_verts
		{
			static void apply(std::vector<int>& out, const std::vector<int>& internal_dynamic_verts, const std::vector<int>& interface_verts)
			{
				out = internal_dynamic_verts;
				out.insert(out.end(), interface_verts.begin(), interface_verts.end());
			}

		};



		struct compute_dynamic_positions
		{
			static void apply(std::vector<vec3>& out, const std::vector<float3>& in )
			{
				out.clear();
				out.reserve(in.size());
				for (int i = 0; i < in.size(); i++)
				{
					int v = i;
					out.push_back({ in[v][0] ,in[v][1] ,in[v][2] });
				}
				out.shrink_to_fit();
			}
		};


		struct get_range
		{
			static void apply(int2& out, const std::vector<int>& in)
			{
				out[0] = 0;
				out[1] = in.size();
			}

			static void apply(int2& out, const std::vector<int>& in,int offset)
			{
				out[0] = offset;
				out[1] = in.size() + offset;
			}
		};


	}


	struct compute_complete_set
	{
		static void apply(std::vector<int>& out, int totalNum, const std::vector<int>& filter)
		{
			std::set<int> filterHelper(filter.begin(), filter.end());
			out.clear();

			for (int i = 0; i < totalNum; i++)
			{
				if (!filterHelper.contains(i))
				{
					out.push_back(i);
				}
			}

			out.shrink_to_fit();
		}
	};
}

