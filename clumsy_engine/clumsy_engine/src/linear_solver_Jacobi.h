


#pragma once

#include "matrix_math/matrix_math.h"
#include "system_equations.h"

#include <vector>

namespace clumsy_engine
{
	struct Element_Equation_Pointer;

	class Linear_Solver_Jacobi
	{
	public:

		std::vector<vec3f> solve(const std::vector<vec3f>& x0, std::vector<Element_Equation> const& equations);

		void solve(vec3f* x, const std::vector<Element_Equation_Pointer>& element_equation_pointers, int vertex_num);

		std::vector<mat3x3f> lhs;

		std::vector<vec3f> rhs;


	};
}


