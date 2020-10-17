



#pragma once

#include "interaction.h" 



namespace clumsy_engine
{

	class Gravity :public Interaction
	{
	public:
		Gravity(int vertex_num,float gx, float gy, float gz, float time_stpe, std::vector<float> masses)
			:m_vertex_num(vertex_num), m_gx(gx), m_gy(gy), m_gz(gz), m_time_step(time_stpe), m_masses(masses)
		{}
		virtual std::vector<Element_Equation> compute_equations() override;
	private:
		int m_vertex_num;
		float m_gx;
		float m_gy;
		float m_gz;
		float m_time_step;
		std::vector<float> m_masses;
	};
}