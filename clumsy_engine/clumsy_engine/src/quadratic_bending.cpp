

#include "quadratic_bending.h"
#include "matrix_math/matrix_math.h"


namespace clumsy_engine
{

	std::vector<stencil> Quadratic_Bending::compute_stencils()
	{
		const auto& triangle_indice = get_value<data::Triangle_Indice>();
		const auto& edge_indice = get_value<data::Edge_Indice>();
		const auto& edge_adj_face = get_value<data::Edge_Adjacent_Triangle>();

		std::vector<stencil> ret;

		for (int i = 0; i < edge_indice.size() / 2; i++)
		{
			stencil st;
			st.push_back(edge_indice[i * 2 + 0]);
			st.push_back(edge_indice[i * 2 + 1]);

			int f0 = edge_adj_face[i * 2 + 0];
			int v_oppse_0 = triangle_indice[f0 * 3 + 0] + triangle_indice[f0 * 3 + 1] + triangle_indice[f0 * 3 + 2] - edge_indice[i * 2 + 0] - edge_indice[i * 2 + 1];
			st.push_back(v_oppse_0);

			int f1 = edge_adj_face[i * 2 + 1];
			int v_oppse_1 = -1;
			if (f1 != -1)
			{
				v_oppse_1 = triangle_indice[f1 * 3 + 0] + triangle_indice[f1 * 3 + 1] + triangle_indice[f1 * 3 + 2] - edge_indice[i * 2 + 0] - edge_indice[i * 2 + 1];
			}
			st.push_back(v_oppse_1);

			if (v_oppse_1 != -1) //only handle interior edge 
			{
				ret.push_back(st);
			}
		}

		return ret;
	}

	Element_Equation Quadratic_Bending::compute_element_equation(stencil st, int ei)
	{
		int vertex_of_stencel_num = st.size();
		Element_Equation ret = Element_Equation::with_size(vertex_of_stencel_num);

		float w[] = { 1,1,-1,-1 };

		float stiff = 1e3f;

		for (int i = 0; i < vertex_of_stencel_num; i++)
		{

			for (int j = 0; j < vertex_of_stencel_num; j++)
			{
				ret.A[i * vertex_of_stencel_num + j] = stiff * w[i] * w[j] * get_identity<3, float>();
			}

			ret.b[i] = get_uniform<3, 1, float>(0.f);
		}

		ret.stencil = st;
		return ret;
	}

}