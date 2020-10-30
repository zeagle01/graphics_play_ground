


#include "gravity.h" 

#include "one_point_stencils.h"


namespace clumsy_engine
{

	std::vector<stencil> Gravity::compute_stencils(std::vector<float> positions, std::vector<int> triangles) 
	{
		One_Point_Stencils st;

		return st(positions.size() / 3);

	}

	Element_Equation Gravity::compute_element_equation(stencil st) 
	{

		Element_Equation ret;

		ret.A = std::vector<float>{ 0.f };

		float mass = 1.f;//TODO

		ret.b = std::vector<float>
		{
			mass * m_gx,
			mass * m_gy,
			mass * m_gz
		};

		ret.stencil = std::vector<int>{ st[0] };

		return ret;

	}


}