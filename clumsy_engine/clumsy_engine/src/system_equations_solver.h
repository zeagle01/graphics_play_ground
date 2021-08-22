


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


	struct To_CSR
	{
	public:
		void operator()(std::vector<Element_Equation> const equations)
		{
			std::vector<std::vector<float>> values;
			//std::vector<int> I;
			std::vector<std::vector<int>> J;

			for (int i = 0; i < equations.size(); i++)
			{
				int en = equations[i].stencil.size();
				for (int di = 0; di < 3; di++)
				{
					for (int si = 0; si < en; si++)
					{
						for (int sj = 0; sj < en; sj++)
						{
							auto vertexMatrix = equations[i].A[si * en + sj];

							int vi = equations[i].stencil[si];
							int vj = equations[i].stencil[sj];
							for (int dj = 0; dj < 3; dj++)
							{
								values[di].push_back(vertexMatrix(di, dj));
								J[vi * 3 + di].push_back(vj * 3 + dj);
							}
						}
					}
				}
			}
		}
	};


}