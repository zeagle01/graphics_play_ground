


#pragma once

#include "matrix_math/matrix_math.h"
#include "system_equations.h"

#include <vector>

namespace clumsy_engine
{
	class Linear_Solver_CG
	{
	public:

		std::vector<vec3f> solve(const std::vector<vec3f>& x0, std::vector<Element_Equation> const& equations);

	};
}

