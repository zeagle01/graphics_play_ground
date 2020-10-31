

#pragma once

#include "interaction.h"
#include "simulator.h"
#include "simulation_data.h"

namespace clumsy_engine
{

	std::vector<Element_Equation> Interaction::compute_element_equations(Simulator* sim)
	{
		auto& stencils = compute_stencils(sim->get<Position>(), sim->get<Triangle_Indices>());
		std::vector<Element_Equation> ret;
		for (auto& st : stencils)
		{
			ret.push_back(compute_element_equation(st));
		}
		return ret;
	};

}
