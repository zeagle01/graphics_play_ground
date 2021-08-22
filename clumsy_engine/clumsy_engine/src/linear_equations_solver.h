
#pragma once

#include "clumsy_lib/type_map.h"
#include "matrix_math/matrix_math.h"
#include "system_equations_solver.h"
#include "clumsy_lib/class_reflection.h"
#include "linear_solver_Jacobi.h"

#include <vector>

namespace clumsy_engine
{

	class Linear_Equations_Solver_Base
	{
	public:
		virtual std::vector<vec3f> solve(std::vector<vec3f> const& x0, std::vector<Element_Equation> const& equations) = 0;
	};


	template<typename Solver_T>
	class Linear_Equations_Solver :public Linear_Equations_Solver_Base
	{
	public:
		std::vector<vec3f> solve(std::vector<vec3f> const& x0, std::vector<Element_Equation> const& equations) override { return m_solver.solve(x0, equations); }

	private:
		Solver_T m_solver;
	};


#define ADD_LINEAR_SOLVER(name,solver_imp) ADD_TYPE_TO_GROUP(name, Linear_Equations_Solver, solver_imp);


	struct linear_solver
	{
		ADD_LINEAR_SOLVER(Jacobi, Linear_Solver_Jacobi);
	};




}

