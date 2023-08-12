



#pragma once

#include "interaction.h"
#include "Simulation_Data.h"
#include <memory>
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/type_map.h"



namespace clumsy_engine
{

	class Gravity : public clumsy_lib::Variable_Acc_Constraint<clumsy_lib::type_list<data::Gravity_Acceleration,data::Mass, data::Vertex_Num>>
	{
	public:

		std::vector<stencil> compute_stencils() ;

		Element_Equation compute_element_equation(stencil st,int i) ;

	};



}