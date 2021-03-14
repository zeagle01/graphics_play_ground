

#include "spring_stretch.h"
#include "simulation_data.h"

namespace clumsy_engine
{

	std::vector<stencil> Spring_Stretch::compute_stencils()
	{
		//TODO: return edge indices of mesh
		std::vector<stencil>ret;

		return { {0,1} };
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