


#include "inertial.h"
#include "Simulation_Data.h"
#include "one_point_stencils.h"


namespace clumsy_engine
{

	std::vector<stencil> Inertial::compute_stencils()
	{
		One_Point_Stencils st;

		return st(get<data::Vertex_Num>());
	}

	Element_Equation Inertial::compute_element_equation(stencil st)
	{

		Element_Equation eq;

		float mass = 1.f;//TODO

		const auto& lastPos = get<data::Last_Frame_Position>();
		const auto& velocity = get<data::Velocity>();

		float time_step = get<data::Time_Step>();

		int v = st[0];

		float k = mass / time_step / time_step;
		eq.A = std::vector<float>{k};

		eq.b = std::vector<float>
		{
			k* lastPos[v * 3 + 0] + time_step * velocity[v * 3 + 0],
			k* lastPos[v * 3 + 1] + time_step * velocity[v * 3 + 1],
			k* lastPos[v * 3 + 2] + time_step * velocity[v * 3 + 2],
		};

		eq.stencil = { v };

		return eq;
	}



}
