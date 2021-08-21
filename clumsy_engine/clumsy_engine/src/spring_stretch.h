

#pragma  once

#include "interaction.h"
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/type_map.h"

namespace clumsy_engine
{
	class Spring_Stretch : public clumsy_lib::Variable_Acc_Constraint<clumsy_lib::type_list<data::Position,data::Edge_Indice,data::Edge_Length,data::Stretch_Stiff>>
	{
	public:
		std::vector<stencil> compute_stencils();
		Element_Equation compute_element_equation(stencil st, int ei);
	};
}