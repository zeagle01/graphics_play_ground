

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

	Element_Equation Spring_Stretch::compute_element_equation(stencil st)
	{
		Element_Equation ret = Element_Equation::with_size(st.size());
		ret.stencil = st;

		const auto& positions = get<data::Position>();

		float w[] = { -1,1 };
		float x[] = {
			positions[st[0] * 3 + 0],
			positions[st[0] * 3 + 1],
			positions[st[0] * 3 + 2],
			positions[st[1] * 3 + 0],
			positions[st[1] * 3 + 1],
			positions[st[1] * 3 + 2]
		};

		float d[3];
		for (int i = 0; i < 3; i++)
		{
			d[i] = 0;
			for (int j = 0; j < 2; j++)
			{
				d[i] += w[j] * x[j * 3 + i];
			}
		}

		float l = 0;
		for (int i = 0; i < 3; i++)
		{
			l += d[i] * d[i];
		}
		l = std::sqrt(l);

		//float l0=m_rest_length[ei]//
		float l0 = 1.f;//TODO
		for (int i = 0; i < 3; i++)
		{
			d[i] *= l0 / l;
		}

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ret.A[i * 2 + j] = w[i] * w[j] * m_stiff;
			}

			for (int j = 0; j < 3; j++)
			{
				ret.b[i * 3 + j] = w[i] * d[j] * m_stiff;
			}
		}



		return ret;
	}

}