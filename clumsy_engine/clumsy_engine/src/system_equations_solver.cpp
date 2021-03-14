


#include "system_equations_solver.h"



namespace clumsy_engine
{

	void System_Equations_Solver::solve(std::vector<vec3f>& ret, std::vector<Element_Equation> const equations)
	{
		auto& x0 = ret;
		int vertexNum = ret.size() ;

		std::vector<mat3x3f> lhs(vertexNum);

		std::vector<vec3f> rhs(vertexNum);


		for (int i = 0; i < equations.size(); i++)
		{
			auto& equation = equations[i];

			int element_vertex_num = equation.stencil.size();

			for (int evi = 0; evi < element_vertex_num; evi++)
			{
				int ev_r = equation.stencil[evi];

				lhs[ev_r] = lhs[ev_r] + equation.A[evi * element_vertex_num + evi];

				rhs[ev_r] = rhs[ev_r] + equation.b[evi];

				for (int evj = 0; evj < element_vertex_num && evj != evi; evj++)
				{

					int ev_c = equation.stencil[evj];

					rhs[ev_r] = rhs[ev_r] - (equation.A[evi * element_vertex_num + evj] & x0[ev_c]);
				}
			}
		}

		for (int i = 0; i < vertexNum; i++)
		{
			ret[i ] = rhs[i ] | lhs[i];
		}

	}


}
