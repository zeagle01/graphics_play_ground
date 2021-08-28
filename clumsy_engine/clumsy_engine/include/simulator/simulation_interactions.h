
#pragma once
#include "clumsy_lib/class_reflection.h"
#include "gravity.h"
#include "spring_stretch.h"
#include "quadratic_bending.h"
#include "inertial.h"


namespace clumsy_engine
{


template<typename Interaction_T>
class Interaction_Wrapper :public Interaction
{
public:
	std::vector<stencil> compute_stencils() override
	{
		return m_imp.compute_stencils();
	}

	Element_Equation compute_element_equation(stencil st, int i) override
	{
		return m_imp.compute_element_equation(st, i);
	}

	void set_variable_set(std::shared_ptr<clumsy_lib::Variable_Set> variables) override
	{
		m_imp.set_type_map(variables);
	}

private:
	Interaction_T  m_imp;
};


#define ADD_INTERACTION(i_name)\
	using I##i_name = i_name; \
	ADD_TYPE_TO_GROUP(i_name, Interaction_Wrapper,I##i_name) 

	struct interaction
	{
		ADD_INTERACTION(Gravity);
		ADD_INTERACTION(Spring_Stretch);
		ADD_INTERACTION(Quadratic_Bending);
		ADD_INTERACTION(Inertial);
	} ;


}
