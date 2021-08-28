

#pragma once

#include "clumsy_lib/class_reflection.h"
#include "linear_equations_solver.h"


namespace clumsy_engine
{


#define ADD_MORPHISM(name) \
		using name = clumsy_engine::name; \
		name* name_##var; \

	struct Morphism
	{
		ADD_MORPHISM(Linear_Equations_Solver);

	};


}