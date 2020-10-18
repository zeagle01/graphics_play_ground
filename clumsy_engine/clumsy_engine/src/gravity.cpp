


#include "gravity.h" 



namespace clumsy_engine
{

	std::vector<Constraint> Gravity::comfigure_constraints(std::shared_ptr<Simulation_Data> sim_data)
	{

		std::vector<Constraint> ret;

		int vertex_num = sim_data->get_vertex_num();

		ret.resize(vertex_num);

		for (int i = 0; i < vertex_num; i++)
		{
			ret[i] = [&sim_data,i,this]()
			{

				Element_Equation ret;

				ret.A = std::vector<float>{ 0.f };

				ret.b = std::vector<float>
				{
					 sim_data->m_masses[i] * m_gx,
					 sim_data->m_masses[i] * m_gy,
					 sim_data->m_masses[i] * m_gz
				};

				ret.stencil = std::vector<int>{ i };

				return ret;
			};
				
		}

		return ret;

	}



}