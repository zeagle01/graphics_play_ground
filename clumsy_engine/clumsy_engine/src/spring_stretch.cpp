

#include "spring_stretch.h"

namespace clumsy_engine
{

	std::vector<stencil> Spring_Stretch::compute_stencils(std::vector<float> positions, std::vector<int> triangles)
	{
		//TODO: return edge indices of mesh
		std::vector<stencil>ret;

		return { {0,1} };
	}

	Element_Equation Spring_Stretch::compute_element_equation(stencil st)
	{
		Element_Equation ret;

		return ret;
	}

}