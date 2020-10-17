


#pragma once

#include "Interaction.h"
#include <vector> 

namespace clumsy_engine
{


	class Inertial :public Interaction
	{
	public:
		Inertial(int vertex_num, std::vector<float> masses, std::vector<float> position0, std::vector<float> velocity, float time_step) :
			m_vertex_num(vertex_num), m_masses(masses), m_position0(position0), m_velocity(velocity), m_time_step(time_step)
		{}

		std::vector<Element_Equation> compute_equations() override;

	private:
		int m_vertex_num;
		std::vector<float> m_masses;
		std::vector<float> m_velocity;
		std::vector<float> m_position0;
		float m_time_step;
	};


}