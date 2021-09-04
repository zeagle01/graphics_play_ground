

#include "linear_solver_Jacobi.h"
#include "profiler.h"
#include "compressed_sparse_row.h"

#include <vector>


namespace clumsy_engine
{

	template<typename T, int N>
	std::vector<vec<N, T>> diagonal_inverse(const CSR<mat<N, N, T>>& A, const std::vector<vec<N, T>>& v)
	{
		std::vector<vec<N, T>> ret(v.size());
		for (int i = 0; i < v.size(); i++)
		{
			if (i == 0)
			{
				ret[i] = get_uniform<N, 1, T>(0);
				continue;
			}

			int rowBegin = A.I[i];
			int colIndex = A.J[rowBegin];
			ret[i] = v[i] | A.values[rowBegin];
		}
		return ret;
	}


	std::vector<vec3f> Linear_Solver_Jacobi::solve(const std::vector<vec3f>& x0, std::vector<Element_Equation> const& equations)
	{
		RECORD_FUNCTION_DURATION();
		std::vector<vec3f> ret = x0;
		int vertexNum = ret.size() ;

		CSR<mat3x3f> A;
		std::vector<vec3f> b(x0.size(), get_uniform<3, 1, float>(0));
		convert_system_equations_to_CSR(A, b, equations);
		auto r = b - A * x0;
		auto delta_x= diagonal_inverse(A, r);
		return x0 + delta_x;
	}

}