

#pragma once

#include "system_equations_solver.h" 
#include "Simulation_Data.h"
#include <memory>
#include <functional>

namespace clumsy_engine
{

//	using Constraint = std::function<Element_Equation()>;
	using stencil = std::vector<int>;

	class Interaction
	{
	public:
		virtual ~Interaction() {}

		virtual std::vector<stencil> compute_stencils(std::vector<float> positions, std::vector<int> triangles) { return std::vector<stencil>(); }

		virtual Element_Equation compute_element_equation(stencil st) { return Element_Equation(); }

		std::vector<Element_Equation> compute_element_equations(std::shared_ptr<Simulation_Data> sim_data) 
		{ 
			auto& stencils = compute_stencils(sim_data->m_positions, sim_data->m_triangles);
			std::vector<Element_Equation> ret;
			for (auto& st : stencils)
			{
				ret.push_back(compute_element_equation(st));
			}
			return ret; 
		};
	};

}
