
#pragma once

#include "clumsy_lib/type_list.h"
#include "clumsy_lib/type_map.h"
#include "clumsy_lib/variable_accecor.h"
#include "simulation_data.h"
#include "interaction.h"

namespace clumsy_engine
{

	class Quadratic_Bending :public clumsy_lib::Variable_Acc_Constraint<clumsy_lib::type_list<data::Position, data::Triangle_Indice, data::Edge_Indice, data::Edge_Adjacent_Triangle>>
	{
	public:
		std::vector<stencil> compute_stencils();
		Element_Equation compute_element_equation(stencil st, int ei);
	};

}
