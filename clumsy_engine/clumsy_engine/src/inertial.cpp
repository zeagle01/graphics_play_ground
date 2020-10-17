


#include "inertial.h"


namespace clumsy_engine
{
	std::vector<Element_Equation> Inertial::compute_equations()
	{

		std::vector<Element_Equation> ret;

		ret.resize(m_vertex_num);

		for (int i = 0; i < m_vertex_num; i++)
		{
			ret[i].A = std::vector<float>{ m_masses[i] / m_time_step / m_time_step };

			ret[i].b = std::vector<float>
			{
				m_position0[i * 3 + 0] + m_time_step * m_velocity[i * 3 + 0],
				m_position0[i * 3 + 1] + m_time_step * m_velocity[i * 3 + 1],
				m_position0[i * 3 + 2] + m_time_step * m_velocity[i * 3 + 2]
			};

			ret[i].stencil = std::vector<int>{ i };
		}

		return ret;
	}

}
