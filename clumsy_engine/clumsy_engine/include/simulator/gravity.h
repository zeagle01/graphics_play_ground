



#pragma once

#include "interaction.h" 
#include "Simulation_Data.h"
#include <memory>
#include "type_list.h"



namespace clumsy_engine
{

	class Gravity :public Interaction, public Simulation_Data_Acc<type_list<data::Gravity,data::Mass, data::Vertex_Num>>
	{
	public:

		std::vector<stencil> compute_stencils() override;

		Element_Equation compute_element_equation(stencil st,int i) override;

	};



}