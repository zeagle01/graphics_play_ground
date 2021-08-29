


#pragma  once

#include <vector> 

#include <functional>
#include "matrix_math/matrix_math.h"
#include "compressed_sparse_row.h"


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

	static void convert_system_equations_to_CSR(CSR<mat3x3f>& A, std::vector<vec3f>& b, std::vector<Element_Equation> const& equations)
	{
		IJV_To_CSR<mat3x3f> convertor;
		for (int i = 0; i < equations.size(); i++)
		{
			int stencil_vertex_num = equations[i].stencil.size();
			for (int si = 0; si < stencil_vertex_num; si++)
			{
				int vi = equations[i].stencil[si];
				b[i] += equations[i].b[si];
				for (int sj = 0; sj < stencil_vertex_num; sj++)
				{
					int vj = equations[i].stencil[sj];
					const auto& v = equations[i].A[si * stencil_vertex_num + sj];
					convertor.add(vi, vj, v);
				}
			}
		}
		A = convertor.get_CSR();
	}

}