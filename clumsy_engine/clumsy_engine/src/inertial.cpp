


#include "inertial.h"
#include "Simulation_Data.h"
#include "one_point_stencils.h"


namespace clumsy_engine
{

	std::vector<stencil> Inertial::compute_stencils(std::vector<float> positions, std::vector<int> triangles)
	{
		One_Point_Stencils st;

		return st(positions.size() / 3);
	}

	Element_Equation Inertial::compute_element_equation(stencil st)
	{

		Element_Equation eq;

		float mass = 1.f;//TODO

		std::vector<float> lastPos{ 0,0,0 };//TODO
		std::vector<float> velocity{ 0,0,0 };//TODO

		eq.A = std::vector<float>{ mass / m_time_step / m_time_step };

		eq.b = std::vector<float>
		{
			lastPos[0] + m_time_step * velocity[0],
			lastPos[1] + m_time_step * velocity[1],
			lastPos[2] + m_time_step * velocity[2],
		};

		eq.stencil = { st[0] };

		return eq;
	}



}
