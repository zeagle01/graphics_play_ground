
#include "data_computes.h"
#include "simulation_data.h"

namespace clumsy_engine
{


	void Delta::apply(Simulation_Datas& datas, std::vector<float>& d)
	{
		const auto& positions = datas.get_data<data::Position>();
		const auto& last_positions = datas.get_data<data::Last_Frame_Position>();

		if (d.empty())
		{
			d.resize(positions.size());
		}

		for (int i = 0; i < positions.size();i++)
		{
			d[i] = positions[i] - last_positions[i];
		}
	}

	void Compute_Edge_Length::apply(Simulation_Datas& datas, std::vector<float>& edge_length)
	{
		const auto& positions = datas.get_data<data::Position>();
		const auto& edge_indices = datas.get_data<data::Edge_Indice>();
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

	void Compute_Edge_Indices::apply(Simulation_Datas& datas, std::vector<int>& d)
	{
		auto triangle_indices = datas.get_data<data::Triangle_Indice>();
		int t_num = triangle_indices.size() / 3;
		int v_num = datas.get_data<data::Vertex_Num>();

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

}
