

#include "linear_solver_Jacobi.h"
#include "profiler.h"


namespace clumsy_engine
{

	std::vector<vec3f> Linear_Solver_Jacobi::solve(const std::vector<vec3f>& x0, std::vector<Element_Equation> const& equations)
	{
		RECORD_FUNCTION_DURATION();
		std::vector<vec3f> ret = x0;
		int vertexNum = ret.size() ;

		if (lhs.size() != vertexNum)
		{
			lhs.resize(vertexNum, get_uniform<3, 3, float>(0));
			rhs.resize(vertexNum, get_uniform<3, 1, float>(0));
		}

		lhs.assign(vertexNum, get_uniform<3, 3, float>(0));
		rhs.assign(vertexNum, get_uniform<3, 1, float>(0));


		for (int i = 0; i < equations.size(); i++)
		{
			auto& equation = equations[i];

			int element_vertex_num = equation.stencil.size();

			for (int evi = 0; evi < element_vertex_num; evi++)
			{
				int ev_r = equation.stencil[evi];

				lhs[ev_r] = lhs[ev_r] + equation.A[evi * element_vertex_num + evi];

				rhs[ev_r] = rhs[ev_r] + equation.b[evi];

				for (int evj = 0; evj < element_vertex_num; evj++)
				{
					if (evj == evi) {
						continue;
					}

					int ev_c = equation.stencil[evj];

					rhs[ev_r] = rhs[ev_r] - (equation.A[evi * element_vertex_num + evj] & x0[ev_c]);
				}
			}
		}

		for (int i = 0; i < vertexNum; i++)
		{

			if (i == 0)
			{
				continue; //fix point
			}
			ret[i] = rhs[i] | lhs[i];
		}
		return ret;
	}

}