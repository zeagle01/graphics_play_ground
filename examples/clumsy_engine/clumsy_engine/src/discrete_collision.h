
#pragma once

#include "interaction.h"

#include "clumsy_lib/type_list.h"

namespace clumsy_engine
{
	class Discrete_Collision_EE :public Interaction, public clumsy_lib::Variable_Acc_Constraint<clumsy_lib::type_list<data::Position, data::Triangle_Indice, data::Edge_Indice>>
	{
	public:
		std::vector<stencil> compute_stencils() override;
		Element_Equation compute_element_equation(stencil st,int ei) override;

	private:
		std::vector<vec4f> m_weights;
		std::vector<vec3f> m_constraint_normal;
		std::vector<float> m_violation_distance;
	};
}
