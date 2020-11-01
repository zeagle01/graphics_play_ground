



#pragma once

#include "interaction.h" 
#include "Simulation_Data.h"
#include <memory>
#include "type_list.h"



namespace clumsy_engine
{

	class Gravity :public Interaction
	{
	public:

		using dependent_variables = type_list<data::Gravity,data::Position>; //TODO

		std::vector<stencil> compute_stencils() override;

		Element_Equation compute_element_equation(stencil st) override;

	};



}