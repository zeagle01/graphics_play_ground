
module;

#include <vector>
#include <ranges>
#include <algorithm>
#include <set>
#include <unordered_map>

module sim_lib : simulator_data_update;

import :sim_data;
import :small_mat;

import matrix_math;


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
			static void apply(std::vector<std::array<T, N>>& out, const std::vector<matrix_math::matrix<T, N, 1>>& in)
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

			template<typename T >
			static void apply(std::vector<T>& out, const std::vector<int>& index, const std::vector<T>& in)
			{
				out.resize(index.size());
				for (int i = 0; i < index.size(); i++)
				{
					int ind = index[i];
					out[i] = in[ind];
				}
			}

			template<typename T, int N>
			static void apply(std::array<T, N>& out, const matrix_math::matrix<T, N, 1>& in)
			{
				for (int j = 0; j < N; j++)
				{
					out[j] = in(j);
				}
			}

			template<typename T, int N>
			static void apply(matrix_math::matrix<T, N, 1 >& out, const std::array<T, N>& in)
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

					out[i] = matrix_math::length(dx);
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

		struct convert_pos
		{
			static void apply(std::vector<vec3f>& out, const std::vector<float3>& in)
			{
				out.resize(in.size());
				for (int i = 0; i < in.size(); i++)
				{
					for (int j = 0; j < 3; j++)
					{
						out[i](j) = in[i][j];
					}
				}
				out.shrink_to_fit();
			}

			static void apply(std::vector<vec2f>& out, const std::vector<float2>& in)
			{
				out.resize(in.size());
				for (int i = 0; i < in.size(); i++)
				{
					for (int j = 0; j < 2; j++)
					{
						out[i](j) = in[i][j];
					}
				}
				out.shrink_to_fit();
			}

		};


	}


}

