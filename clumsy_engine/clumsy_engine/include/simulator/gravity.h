



#pragma once

#include "interaction.h" 
#include "Simulation_Data.h"
#include <memory>
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/type_map.h"



namespace clumsy_engine
{

	class Gravity :public Interaction, public clumsy_lib::Variable_Acc_Constraint<clumsy_lib::type_list<data::Gravity,data::Mass, data::Vertex_Num>>
	{
	public:

		std::vector<stencil> compute_stencils() override;

		Element_Equation compute_element_equation(stencil st,int i) override;

	};



}