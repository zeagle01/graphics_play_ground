

#pragma once

#include "interaction.h"
#include "simulator.h"
#include "simulation_data.h"
#include "type_map.h"

namespace clumsy_engine
{

	std::vector<Element_Equation> Interaction::compute_element_equations()
	{
		const auto& stencils = compute_stencils();
		std::vector<Element_Equation> ret;
		for (auto& st : stencils)
		{
			ret.push_back(compute_element_equation(st));
		}
		return ret;
	};

}
