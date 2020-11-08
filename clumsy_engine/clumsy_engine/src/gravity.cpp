


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

		int v = st[0];

		const auto& gravity = get<data::Gravity>();
		const auto& mass = get<data::Mass>();

		float mass_v = mass[v];//TODO

		ret.b = std::vector<float>
		{
			mass_v * gravity[0],
			mass_v * gravity[1],
			mass_v * gravity[2]
		};

		ret.stencil = std::vector<int>{v};

		return ret;

	}


}