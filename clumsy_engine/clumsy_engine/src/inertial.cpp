


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

	Element_Equation Inertial::compute_element_equation(stencil st,int ei)
	{

		Element_Equation eq;


		const auto& lastPos = get<data::Last_Frame_Position>();
		const auto& velocity = get<data::Velocity>();
		const auto& mass = get<data::Mass>();

		float time_step = get<data::Time_Step>();

		int v = st[0];

		float mass_v = mass[v];//TODO

		float k = mass_v / time_step / time_step;

		eq.A = std::vector<mat3x3f>{ get_identity<3,float>() * k };

		eq.b = std::vector<vec3f>
		{
			k* lastPos[v] + time_step * velocity[v],
		};

		eq.stencil = { v };

		return eq;
	}



}
