

#pragma once
#include <cmath>

#include "matrix_math/matrix_math.h"

namespace clumsy_engine
{


	struct data;

	template<typename T,typename U>
	void Resize_If_Empty(T& dst,const U& src )
	{
		if (dst.empty())
		{
			dst.resize(src.size());
		}
	}

	template<typename L, typename V, int N = 1>
	struct Allocate_With_Size {
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, L& d)
		{
			auto size = datas.get<V>();
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
			d = datas.get<L>().size()/Dim;
		}
	};


	struct Delta {
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<float>& d)
		{
			const auto& positions = datas.get<data::Position>();
			const auto& last_positions = datas.get<data::Last_Frame_Position>();

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

			const auto& positions = datas.get<data::Position>();
			const auto& edge_indices = datas.get<data::Edge_Indice>();
			int eNum = edge_indices.size() / 2;
			edge_length.resize(eNum);
			for (int i = 0; i < eNum; i++)
			{
				int v0 = edge_indices[i * 2 + 0];
				int v1 = edge_indices[i * 2 + 1];
				float l = 0;
				for (int i = 0; i < 3; i++)
				{
					float d = positions[v0 * 3 + i] - positions[v1 * 3 + i];
					l += d * d;
				}
				edge_length[i] = std::sqrt(l);
			}

		}
	};

	struct Compute_Edge_Indices { 

		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<int>& d)
		{
			auto triangle_indices = datas.get<data::Triangle_Indice>();
			int t_num = triangle_indices.size() / 3;
			int v_num = datas.get<data::Vertex_Num>();

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

		}
	};

	struct Compute_Triangle_Area
	{
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<float>& triangle_area)
		{
			const auto& triangle_indice = datas.get<data::Triangle_Indice>();
			const auto& position = datas.get<data::Position>();

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

	struct Compute_Vertex_Area
	{
		template<typename sim_acc_T>
		static void apply(sim_acc_T& datas, std::vector<float>& vertex_area)
		{
			auto triangle_area = datas.get<data::Triangle_Area>();
			auto triangle_indice = datas.get<data::Triangle_Indice>();
			auto v_num = datas.get<data::Vertex_Num>();
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
			auto vertex_areas = datas.get<data::Vertex_Area>();
			auto density = datas.get<data::Mass_Density>();

			Resize_If_Empty(mass, vertex_areas);

			for (int i = 0; i < vertex_areas.size(); i++)
			{
				mass[i] = density * vertex_areas[i];
			}

		}
	};

}


