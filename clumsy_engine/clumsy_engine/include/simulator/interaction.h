

#pragma once

#include "system_equations_solver.h" 
#include "Simulation_Data.h"
#include <memory>
#include <functional>

namespace clumsy_engine
{

	class Simulator;

	using stencil = std::vector<int>;

	class Interaction
	{
	public:
		virtual ~Interaction() {}

		virtual std::vector<stencil> compute_stencils() { return std::vector<stencil>(); }

		virtual Element_Equation compute_element_equation(stencil st,int i) { return Element_Equation(); }

		std::vector<Element_Equation> compute_element_equations();
	};

}
