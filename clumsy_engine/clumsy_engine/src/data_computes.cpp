
#include "data_computes.h"
#include "simulation_data.h"

namespace clumsy_engine
{


	void Compute_Last_Frame_Position::apply(Simulation_Datas& datas, std::vector<float>& d)
	{
		Copy_If_Empty::apply(datas.get_data<data::Position>(), d);
	}

	void Delta::apply(Simulation_Datas& datas, std::vector<float>& d)
	{
		const auto& positions = datas.get_data<data::Position>();
		const auto& last_positions = datas.get_data<data::Last_Frame_Position>();

		Copy_If_Empty::apply(positions, d);

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

}
