


#pragma once

#include "Interaction.h"
#include "Simulation_Data.h"
#include <vector> 
#include "type_list.h"

namespace clumsy_engine
{


	class Inertial :public Interaction
	{
	public:

		using dependent_variables = type_list<data::Position, data::Time_Step>; 


		std::vector<stencil> compute_stencils() override;

		Element_Equation compute_element_equation(stencil st) override;


	};

}