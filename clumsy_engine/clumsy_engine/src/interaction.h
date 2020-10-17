

#pragma once

#include "system_equations_solver.h" 

namespace clumsy_engine
{
	class Interaction
	{
	public:
		virtual ~Interaction() {}
		virtual std::vector<Element_Equation> compute_equations() { return std::vector<Element_Equation>(); }
	};
}
