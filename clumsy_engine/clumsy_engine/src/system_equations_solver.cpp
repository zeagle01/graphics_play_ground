


#include "system_equations_solver.h"



namespace clumsy_engine
{

	void System_Equations_Solver::solve(std::vector<float>& ret, std::vector<Element_Equation> const equations)
	{
		int vertexNum = ret.size() / 3;

		std::vector<float> diag(vertexNum);

		std::vector<float> off(vertexNum*3);


		for (int i = 0; i < equations.size(); i++)
		{
			auto& equation = equations[i];

			int element_vertex_num = equation.stencil.size();

			for (int evi = 0; evi < element_vertex_num; evi++)
			{
				int ev_r = equation.stencil[evi];

				diag[ev_r] += equation.A[evi * element_vertex_num + evi];

				off[ev_r * 3 + 0] += equation.b[evi * 3 + 0];
				off[ev_r * 3 + 1] += equation.b[evi * 3 + 1];
				off[ev_r * 3 + 2] += equation.b[evi * 3 + 2];

				for (int evj = 0; evj < element_vertex_num && evj != evi; evj++)
				{

					int ev_c = equation.stencil[evj];

					off[ev_r * 3 + 0] -= equation.A[evi * element_vertex_num + evj] * ret[ev_c * 3 + 0];
					off[ev_r * 3 + 1] -= equation.A[evi * element_vertex_num + evj] * ret[ev_c * 3 + 1];
					off[ev_r * 3 + 2] -= equation.A[evi * element_vertex_num + evj] * ret[ev_c * 3 + 2];
				}
			}
		}

		for (int i = 0; i < vertexNum; i++)
		{
			ret[i * 3 + 0] = off[i * 3 + 0] / diag[i];
			ret[i * 3 + 1] = off[i * 3 + 1] / diag[i];
			ret[i * 3 + 2] = off[i * 3 + 2] / diag[i];
		}

	}


}
