


#pragma  once

#include <vector> 

#include <functional>
#include "matrix_math/matrix_math.h"


namespace clumsy_engine
{


	struct Element_Equation
	{

		std::vector<mat3x3f> A;
		std::vector<vec3f> b;
		std::vector<int> stencil;

		friend bool operator==(const Element_Equation& l, const Element_Equation& r)
		{
			if (l.stencil.size() != r.stencil.size())
			{
				return false;
			}

			int n = l.stencil.size();

			for (int i = 0; i < n; i++)
			{

				if (l.stencil[i] != r.stencil[i])
				{
					return false;
				}

				if (!is_near<0>(l.b[i], r.b[i]))
				{
					return false;
				}

				for (int j = 0; j < n; j++)
				{
					if (!is_near<0>(l.A[i * n + j], r.A[i * n + j]))
					{
						return false;
					}
				}


			}
			return true;
		}
		static Element_Equation with_size(int vNum)
		{
			Element_Equation ret;
			ret.A.resize(vNum * vNum);
			ret.b.resize(vNum);
			ret.stencil.resize(vNum);
			return ret;
		}
	};

	class System_Equations_Solver
	{
	public:

		virtual void solve(std::vector<vec3f>& x, std::vector<Element_Equation> const equations);

	};


}