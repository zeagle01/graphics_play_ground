


#pragma once

#include "interaction.h"
#include "Simulation_Data.h"
#include <vector> 
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/type_map.h"
#include "dependent_variable_set.h"

namespace clumsy_engine
{


	class Inertial : public clumsy_lib::Variable_Acc_Constraint<clumsy_lib::type_list< data::Vertex_Num, data::Mass, data::Last_Frame_Position, data::Velocity, data::Time_Step>>
	{
	public:

		std::vector<stencil> compute_stencils() ;

		Element_Equation compute_element_equation(stencil st,int ei) ;


	};

}