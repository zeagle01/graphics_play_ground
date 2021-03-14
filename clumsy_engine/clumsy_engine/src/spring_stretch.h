

#pragma  once

#include "interaction.h"
#include "type_list.h"

namespace clumsy_engine
{
	class Spring_Stretch :public Interaction, public Simulation_Data_Acc<type_list<data::Position,data::Edge_Length,data::Stretch_Stiff>>
	{
	public:

		std::vector<stencil> compute_stencils() override;

		Element_Equation compute_element_equation(stencil st,int ei) override;



	};
}