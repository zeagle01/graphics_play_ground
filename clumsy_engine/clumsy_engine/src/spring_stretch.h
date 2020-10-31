

#pragma  once

#include "interaction.h"

namespace clumsy_engine
{
	class Spring_Stretch :public Interaction
	{
	public:

		std::vector<stencil> compute_stencils(std::vector<float> positions, std::vector<int> triangles) override;

		Element_Equation compute_element_equation(stencil st) override;


	public:

		void set_positions(const std::vector<float>& positions)
		{
			m_positions = positions;
		}

		void set_stiff(float stiff)
		{
			m_stiff = stiff;
		}

		void set_rest_length(const std::vector<float>& rest_length)
		{
			m_rest_length = rest_length;
		}

	private:
		std::vector<float> m_positions;
		std::vector<float> m_rest_length;
		float m_stiff = 1.f;

	};
}