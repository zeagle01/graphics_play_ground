

#pragma  once

#include "interaction.h"

namespace clumsy_engine
{
	class Spring_Stretch :public Interaction
	{
	public:

		std::vector<stencil> compute_stencils(std::vector<float> positions, std::vector<int> triangles) override;

		Element_Equation compute_element_equation(stencil st) override;


	};
}