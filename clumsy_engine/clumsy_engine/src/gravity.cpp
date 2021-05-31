


#include "gravity.h" 

#include "one_point_stencils.h"


namespace clumsy_engine
{

	std::vector<stencil> Gravity::compute_stencils() 
	{
		One_Point_Stencils st;


		return st(get_value<data::Vertex_Num>());

	}

	Element_Equation Gravity::compute_element_equation(stencil st, int i)
	{

		Element_Equation ret;

		ret.A = std::vector<mat3x3f>{ get_uniform < 3,3,float>(0) };

		int v = st[0];

		const auto& gravity = get_value<data::Gravity>();
		const auto& mass = get_value<data::Mass>();

		float mass_v = mass[v];//TODO

		ret.b = std::vector<vec3f>
		{
			mass_v * gravity,
		};

		ret.stencil = std::vector<int>{v};

		return ret;

	}


}