

#pragma once

#include "system_equations.h" 
#include "Simulation_Data.h"
#include <memory>
#include <functional>

namespace clumsy_engine
{

	class Simulator;

	using stencil = std::vector<int>;

	struct Element_Equation_Pointer
	{
		mat3x3f* A;
		vec3f* b;
		int* element_indices;
		int vertex_num_per_element;
		int element_count;
	};

	class Interaction
	{
	public:
		virtual ~Interaction() {}

		virtual std::vector<stencil> compute_stencils() { return std::vector<stencil>(); }

		virtual Element_Equation compute_element_equation(stencil st,int i) { return Element_Equation(); }

		virtual void set_variable_set(std::shared_ptr<clumsy_lib::Variable_Set>) {};

		std::vector<Element_Equation> compute_element_equations();

		Element_Equation_Pointer compute_element_equations_pointers() { return {}; }
	};

}
