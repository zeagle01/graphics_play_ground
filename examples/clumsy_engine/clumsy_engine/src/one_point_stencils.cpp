

#include "one_point_stencils.h"

namespace clumsy_engine
{
	std::vector<stencil> One_Point_Stencils::operator()(int n)
	{
		if (m_stencils.size() != n)
		{
			m_stencils.resize(n);
			for (int i = 0; i < n; i++)
			{
				m_stencils[i] = { i };

			}
		}


		return m_stencils;
	}

}
