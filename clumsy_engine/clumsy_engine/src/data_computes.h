

#pragma once
#include <cmath>

#include "matrix_math/matrix_math.h"

namespace clumsy_engine
{


	struct data;

	template<typename T,typename U>
	void Resize_If_Empty(T& dst,const U& src )
	{
		if (dst.size() != src.size())
		{
			dst.resize(src.size());
		}
	}

	template<typename L, typename V, int N = 1>
	struct Allocate_With_Size {

		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, L& d)
		{
			auto size = datas.template get_value<V>();
			if (d.empty())
			{
				d.resize(size * N);
			}
		}
	};

	template<typename L,int Dim>
	struct Get_List_Size {

		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, int& d)
		{
			d = datas.template get_value<L>().size()/Dim;
		}
	};


	struct Delta {
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<vec3f>& d)
		{
			const auto& positions = datas.template get_value<data::Position>();
			const auto& last_positions = datas.template get_value<data::Last_Frame_Position>();

			if (d.empty())
			{
				d.resize(positions.size());
			}

			for (int i = 0; i < positions.size(); i++)
			{
				d[i] = positions[i] - last_positions[i];
			}
		}
	};

	struct Compute_Edge_Length {

		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<float>& edge_length)
		{

			const auto& positions = datas.template get_value<data::Ref_Position>();
			const auto& edge_indices = datas.template get_value<data::Edge_Indice>();
			int eNum = edge_indices.size() / 2;
			edge_length.resize(eNum);
			for (int i = 0; i < eNum; i++)
			{
				int v0 = edge_indices[i * 2 + 0];
				int v1 = edge_indices[i * 2 + 1];
				edge_length[i] = Vectorized_Norm<2>()(positions[v0] - positions[v1]);
			}

		}
	};

	struct Compute_Edge_Indices { 

		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<int>& d)
		{
			auto triangle_indices = datas.template get_value<data::Triangle_Indice>();
			int t_num = triangle_indices.size() / 3;
			int v_num = datas.template get_value<data::Vertex_Num>();

			d.clear();
			d.reserve(v_num * 3);

			std::vector<std::vector<int>> vv(v_num);

			for (int ti = 0; ti < t_num; ti++)
			{
				for (int tvi = 0; tvi < 3; tvi++)
				{
					int vi = tvi;
					int vi_next = (tvi + 1) % 3;

					int v = triangle_indices[ti * 3 + vi];
					int v_next = triangle_indices[ti * 3 + vi_next];

					if (std::find(vv[v].begin(), vv[v].end(), v_next) == vv[v].end())
					{
						vv[v_next].push_back(v);

						d.push_back(v);
						d.push_back(v_next);
					}
				}

			}
			d.shrink_to_fit();

		}
	};

	struct Compute_Triangle_Area
	{
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<float>& triangle_area)
		{
			const auto& triangle_indice = datas.template get_value<data::Triangle_Indice>();
			const auto& position = datas.template get_value<data::Ref_Position>();

			int t_num = triangle_indice.size() / 3;
			if (triangle_area.size() != t_num)
			{
				triangle_area.resize(t_num);
			}
			for (int i = 0; i < t_num; i++)
			{
				int v[]{ triangle_indice[i * 3 + 0],triangle_indice[i * 3 + 1],triangle_indice[i * 3 + 2] };
				vec3f x[]{
					position[v[0]],
					position[v[1]],
					position[v[2]]
				};

				auto n = (x[0] - x[2]) ^ (x[1] - x[2]);

				float l = Vectorized_Norm<2>()(n);

				l = 0.5f * l;

				triangle_area[i] = l;
			}

		}
	};

	struct Compute_Triangle_Normal
	{
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<vec3f>& triangle_normal)
		{
			const auto& triangle_indice = datas.template get_value<data::Triangle_Indice>();
			const auto& position = datas.template get_value<data::Position>();

			int t_num = triangle_indice.size() / 3;
			if (triangle_normal.size() != t_num)
			{
				triangle_normal.resize(t_num);
			}

			for (int i = 0; i < t_num; i++)
			{
				int tv[]{ triangle_indice[i * 3 + 0],triangle_indice[i * 3 + 1],triangle_indice[i * 3 + 2] };
				vec3f x[]{
					position[tv[0]],
					position[tv[1]],
					position[tv[2]]
				};

				auto n = (x[0] - x[2]) ^ (x[1] - x[2]);
				triangle_normal[i] = normalize(n);
			}
		}
	};

	struct Compute_Vertex_Adjacent_Triangle
	{
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<std::vector<int>>& vertex_ajd_triangle)
		{

			auto triangle_indices = datas.template get_value<data::Triangle_Indice>();
			int t_num = triangle_indices.size() / 3;
			int v_num = datas.template get_value<data::Vertex_Num>();

			vertex_ajd_triangle.assign(v_num, std::vector<int>());

			std::vector<std::vector<int>> vv(v_num);

			for (int ti = 0; ti < t_num; ti++)
			{
				for (int tvi = 0; tvi < 3; tvi++)
				{
					int vi = tvi;
					int tv = triangle_indices[ti * 3 + vi];

					vertex_ajd_triangle[tv].push_back(ti);
				}
			}
		}
	};

	struct Compute_Vertex_Normal
	{
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<vec3f>& vertex_normal)
		{
			auto const& triangle_normal = datas.template get_value<data::Triangle_Normal>();
			auto const& vertext_adj_triangle = datas.template get_value<data::Vertex_Adjacent_Triangle>();
			auto const& v_num = datas.template get_value<data::Vertex_Num>();

			vertex_normal.assign(v_num, vec3f{ 0, 0, 0 });

			for (int vi = 0; vi < v_num; vi++)
			{
				int adj_triangle_num = vertext_adj_triangle[vi].size();
				for (int ti = 0; ti < adj_triangle_num; ti++)
				{
					const auto& t = vertext_adj_triangle[vi][ti];
					vertex_normal[vi] += triangle_normal[t];
				}
				vertex_normal[vi] = normalize(vertex_normal[vi]);
			}

		}
	};


	struct Compute_Vertex_Area
	{
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<float>& vertex_area)
		{
			auto triangle_area = datas.template get_value<data::Triangle_Area>();
			auto triangle_indice = datas.template get_value<data::Triangle_Indice>();
			auto v_num = datas.template get_value<data::Vertex_Num>();
			if (vertex_area.size() != v_num)
			{
				vertex_area.resize(v_num);
			}

			for (int i = 0; i < triangle_area.size(); i++)
			{
				for (int vi = 0; vi < 3; vi++)
				{
					int v = triangle_indice[i * 3 + vi];
					vertex_area[v] += triangle_area[i] / 3;
				}
			}

		}
	};

	struct Compute_Mass { 

		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<float>& mass)
		{
			auto vertex_areas = datas.template get_value<data::Vertex_Area>();
			auto density = datas.template get_value<data::Mass_Density>();

			Resize_If_Empty(mass, vertex_areas);

			for (int i = 0; i < vertex_areas.size(); i++)
			{
				mass[i] = density * vertex_areas[i];
			}

		}
	};

}


