


#include "gravity.h" 

#include "one_point_stencils.h"


namespace clumsy_engine
{

	std::vector<stencil> Gravity::compute_stencils() 
	{
		One_Point_Stencils st;


		return st(get<data::Vertex_Num>());

	}

	Element_Equation Gravity::compute_element_equation(stencil st) 
	{

		Element_Equation ret;

		ret.A = std::vector<float>{ 0.f };

		float mass = 1.f;//TODO

		const auto& gravity = get<data::Gravity>();

		ret.b = std::vector<float>
		{
			mass * gravity[0],
			mass * gravity[1],
			mass * gravity[2]
		};

		ret.stencil = std::vector<int>{ st[0] };

		return ret;

	}


}