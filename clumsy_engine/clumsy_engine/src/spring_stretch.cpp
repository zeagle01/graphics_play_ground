

#include "spring_stretch.h"
#include "simulation_data.h"

namespace clumsy_engine
{

	std::vector<stencil> Spring_Stretch::compute_stencils()
	{

		const auto& edge_indice = get<data::Edge_Indice>();

		std::vector<stencil>ret;
		for (int i = 0; i < edge_indice.size() / 2; i++)
		{
			ret.push_back({ edge_indice[i * 2 + 0],edge_indice[i * 2 + 1] });
		}

		return ret;
	}

	Element_Equation Spring_Stretch::compute_element_equation(stencil st,int ei)
	{
		Element_Equation ret = Element_Equation::with_size(st.size());
		ret.stencil = st;

		const auto& positions = get<data::Position>();
		const auto& edge_lengths = get<data::Edge_Length>();
		auto stiff = get<data::Stretch_Stiff>();

		vec2f w = { -1,1 };
		mat3x2f x{
			positions[st[0]],
			positions[st[1]]
		};

		vec3f d = x & w;

		float l = Vectorized_Norm<2>()(d);

		d = d * edge_lengths[ei] / l;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ret.A[i * 2 + j] = w(i) * w(j) * stiff * get_identity<3, float>();
			}

			ret.b[i] = w(i) * d * stiff;
		}

		return ret;
	}

}