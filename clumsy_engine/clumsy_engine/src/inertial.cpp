


#include "inertial.h"
#include "Simulation_Data.h"


namespace clumsy_engine
{

	std::vector<Constraint> Inertial::comfigure_constraints(std::shared_ptr<Simulation_Data> sim_data)
	{
		std::vector<Constraint> ret;

		int vertex_num = sim_data->get_vertex_num();
		ret.resize(vertex_num);

		for (int i = 0; i < vertex_num; i++)
		{

			ret[i] = [&sim_data, this, i]()
			{
				Element_Equation eq;
				eq.A = std::vector<float>{ sim_data->m_masses[i] / m_time_step / m_time_step };

				eq.b = std::vector<float>
				{
					sim_data->m_last_frame_positions[i * 3 + 0] + m_time_step * sim_data->m_velocities[i * 3 + 0],
					sim_data->m_last_frame_positions[i * 3 + 1] + m_time_step * sim_data->m_velocities[i * 3 + 1],
					sim_data->m_last_frame_positions[i * 3 + 2] + m_time_step * sim_data->m_velocities[i * 3 + 2],
				};

				eq.stencil = std::vector<int>{ i };

				return eq;
			};

		}

		return ret;
	}


}
