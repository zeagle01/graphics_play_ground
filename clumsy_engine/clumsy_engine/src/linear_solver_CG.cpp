
#include "linear_solver_CG.h"
#include "profiler.h"
#include "matrix_math/matrix_math.h"


namespace clumsy_engine
{


	float dot(const std::vector<vec3f>& v0, const std::vector<vec3f>& v1)
	{
		float ret = 0.f;
		for (int i = 0; i < v0.size(); i++)
		{
			ret += transpose(v0[i]) * v1[i];
		}
		return ret;
	}


	template<typename T,int N>
	std::vector<vec<N, T>> operator*(T s, const std::vector<vec<N, T>>& v)
	{
		std::vector<vec<N, T>> ret(v.size());
		for (int i = 0; i < v.size(); i++)
		{
			ret[i] = s * v[i];
		}
		return ret;
	}



	std::vector<vec3f> Linear_Solver_CG::solve(const std::vector<vec3f>& x0, std::vector<Element_Equation> const& equations)
	{
		RECORD_FUNCTION_DURATION();

		std::vector<vec3f> ret = x0;
		CSR<mat3x3f> A;
		std::vector<vec3f> b(x0.size(), get_uniform<3, 1, float>(0));
		convert_system_equations_to_CSR(A, b, equations);

		auto r = b - A * ret;
		auto p = r;

		int iter_name = 5;
		for (int i = 0; i < iter_name; i++)
		{
			float rr_old = dot(r, r);
			float alpha = rr_old / dot(p, A * p);
			ret = ret + alpha * p;

			ret[0] = x0[0];

			r = r - alpha * (A * p);

			float beta = dot(r, r) / rr_old;

			p = r + beta * p;
		}

		return ret;
	}

}
