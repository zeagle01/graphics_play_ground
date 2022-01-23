
#pragma once

#include "clumsy_lib/type_map.h"
#include "matrix_math/matrix_math.h"
#include "system_equations.h"
#include "clumsy_lib/class_reflection.h"
#include "linear_solver_Jacobi.h"
#include "linear_solver_CG.h"

#include <vector>

namespace clumsy_engine
{

	class Linear_Equations_Solver_Base
	{
	public:
		virtual std::vector<vec3f> solve(std::vector<vec3f> const& x0, std::vector<Element_Equation> const& equations) = 0;
		virtual void solve(vec3f* x, const std::vector<Element_Equation_Pointer>& element_equation_pointers, int vertex_num) = 0;
	};


	template<typename Solver_T>
	class Linear_Equations_Solver_Wrapper :public Linear_Equations_Solver_Base
	{
	public:
		std::vector<vec3f> solve(std::vector<vec3f> const& x0, std::vector<Element_Equation> const& equations) override { return m_solver.solve(x0, equations); }
		void solve(vec3f* x, const std::vector<Element_Equation_Pointer>& element_equation_pointers, int vertex_num) override 
		{
			return m_solver.solve(x, element_equation_pointers, vertex_num);
		};

	private:
		Solver_T m_solver;
	};





#define ADD_LINEAR_SOLVER(name,solver_imp) ADD_TYPE_TO_GROUP(name, Linear_Equations_Solver_Wrapper, solver_imp);

	struct Linear_Equations_Solver
	{
		using base_type = Linear_Equations_Solver_Base;

		ADD_LINEAR_SOLVER(Jacobi, Linear_Solver_Jacobi);
		ADD_LINEAR_SOLVER(Conjugate_Gradient, Linear_Solver_CG);
	};

}

