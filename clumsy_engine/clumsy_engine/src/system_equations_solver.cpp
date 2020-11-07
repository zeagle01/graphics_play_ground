


#include "system_equations_solver.h"



namespace clumsy_engine
{

	void System_Equations_Solver::solve(std::vector<float>& ret, std::vector<Element_Equation> const equations)
	{
		auto& x0 = ret;
		int vertexNum = ret.size() / 3;

		std::vector<float> lhs(vertexNum);

		std::vector<float> rhs(vertexNum*3);


		for (int i = 0; i < equations.size(); i++)
		{
			auto& equation = equations[i];

			int element_vertex_num = equation.stencil.size();

			for (int evi = 0; evi < element_vertex_num; evi++)
			{
				int ev_r = equation.stencil[evi];

				lhs[ev_r] += equation.A[evi * element_vertex_num + evi];

				rhs[ev_r * 3 + 0] += equation.b[evi * 3 + 0];
				rhs[ev_r * 3 + 1] += equation.b[evi * 3 + 1];
				rhs[ev_r * 3 + 2] += equation.b[evi * 3 + 2];

				for (int evj = 0; evj < element_vertex_num && evj != evi; evj++)
				{

					int ev_c = equation.stencil[evj];

					rhs[ev_r * 3 + 0] -= equation.A[evi * element_vertex_num + evj] * x0[ev_c * 3 + 0];
					rhs[ev_r * 3 + 1] -= equation.A[evi * element_vertex_num + evj] * x0[ev_c * 3 + 1];
					rhs[ev_r * 3 + 2] -= equation.A[evi * element_vertex_num + evj] * x0[ev_c * 3 + 2];
				}
			}
		}

		for (int i = 0; i < vertexNum; i++)
		{
			ret[i * 3 + 0] = rhs[i * 3 + 0] / lhs[i];
			ret[i * 3 + 1] = rhs[i * 3 + 1] / lhs[i];
			ret[i * 3 + 2] = rhs[i * 3 + 2] / lhs[i];
		}

	}


}
