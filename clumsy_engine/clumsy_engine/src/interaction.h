

#pragma once

#include "system_equations_solver.h" 
#include "Simulation_Data.h"
#include <memory>
#include <functional>

namespace clumsy_engine
{

	using Constraint = std::function<Element_Equation()>;

	class Interaction
	{
	public:
		virtual ~Interaction() {}

		virtual std::vector<Constraint> comfigure_constraints(std::shared_ptr<Simulation_Data> sim_data) { return std::vector<Constraint>{}; };
	};

}
