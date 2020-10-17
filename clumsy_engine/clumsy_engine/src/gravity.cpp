


#include "gravity.h" 



namespace clumsy_engine
{

	std::vector<Element_Equation> Gravity::compute_equations() 
	{

		std::vector<Element_Equation> ret;

		ret.resize(m_vertex_num);

		for (int i = 0; i < m_vertex_num; i++)
		{
			ret[i].A = std::vector<float>{0.f};

			ret[i].b = std::vector<float>
			{
				 m_masses[i] * m_gx,
				 m_masses[i] * m_gy,
				 m_masses[i] * m_gz
			};

			ret[i].stencil = std::vector<int>{ i };
		}

		return ret;
	}
}