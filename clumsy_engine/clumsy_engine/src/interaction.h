

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

		virtual std::vector<stencil> compute_stencils(std::vector<float> positions, std::vector<int> triangles) { return std::vector<stencil>(); }

		virtual Element_Equation compute_element_equation(stencil st) { return Element_Equation(); }

		std::vector<Element_Equation> compute_element_equations(Simulator* sim);
	};

}
