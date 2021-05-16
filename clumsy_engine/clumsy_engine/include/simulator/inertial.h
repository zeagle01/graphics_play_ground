


#pragma once

#include "Interaction.h"
#include "Simulation_Data.h"
#include <vector> 
#include "clumsy_lib/type_list.h"

namespace clumsy_engine
{


	class Inertial :public Interaction, public Simulation_Data_Acc<clumsy_lib::type_list< data::Vertex_Num, data::Mass, data::Last_Frame_Position, data::Velocity, data::Time_Step>>
	{
	public:

		std::vector<stencil> compute_stencils() override;

		Element_Equation compute_element_equation(stencil st,int ei) override;


	};

}