

#include "discrete_collision.h"
#include "compute_hessian_gradient.h"


namespace clumsy_engine
{

	std::vector<stencil> Discrete_Collision::compute_stencils()
	{
		//mock data
		m_weights = { {-0.5f,-0.5f,0.5f,0.5f} };
		m_constraint_normal = { {0.f,0.f,1.f} };
		m_violation_distance = { 1e-3f};
		return { {0,1,2,3} };

	}

	Element_Equation Discrete_Collision::compute_element_equation(stencil st, int ei)
	{
		int vertex_of_stencel_num = st.size();
		Element_Equation ret = Element_Equation::with_size(vertex_of_stencel_num);
		ret.stencil = st;

		auto weight = m_weights[ei];
		auto constraint_normal = m_constraint_normal[ei];
		auto violation_distance = m_violation_distance[ei];
		float stiff = 1e2f;

		Compute_Hessian_Gradient::apply<4>(ret.A, ret.b, weight, constraint_normal, violation_distance, stiff);

		return ret;
	}

}