
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

			template<typename T,typename U>
			static void apply(std::vector<T>& out, const std::vector<U>& in)
			{
				out.resize(in.size());
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

		struct compute_dynamic_stencils
		{
			template<typename T,int N>
			static void apply(std::vector<std::array<T, N>>& out, const std::vector<std::array<T, N>>& in, const std::vector<int>& dynamic_verts)
			{
				out.clear();

				std::unordered_map<int, int> orig_to_dynmaic;
				for (int i = 0; i < dynamic_verts.size(); i++)
				{
					orig_to_dynmaic[dynamic_verts[i]] = i;
				}

				for (int i = 0; i < in.size(); i++)
				{
					bool is_dynamic_0 = orig_to_dynmaic.contains(in[i][0]);
					for (int j = 1; j < N; j++)
					{
						bool is_dynamic_j = orig_to_dynmaic.contains(in[i][j]);
						if (is_dynamic_j != is_dynamic_0)
						{
							printf(" %d is %d, %d is %d\n", in[i][0], int(is_dynamic_0), in[i][j], int(is_dynamic_j));
							break;

						}
					}

					if (is_dynamic_0)
					{
						std::array<T, N> dynamic_stencil;
						for (int j = 0; j < N; j++)
						{
							dynamic_stencil[j] = orig_to_dynmaic[in[i][j]];
						}
						out.push_back(dynamic_stencil);
					}
				}
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

