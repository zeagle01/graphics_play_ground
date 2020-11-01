

#pragma  once

#include "interaction.h"
#include "type_list.h"

namespace clumsy_engine
{
	class Spring_Stretch :public Interaction
	{
	public:
		using dependent_variables = type_list<data::Position>; 

		std::vector<stencil> compute_stencils() override;

		Element_Equation compute_element_equation(stencil st) override;


	public:

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