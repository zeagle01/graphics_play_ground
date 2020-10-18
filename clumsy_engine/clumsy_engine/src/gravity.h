



#pragma once

#include "interaction.h" 
#include "Simulation_Data.h"
#include <memory>



namespace clumsy_engine
{

	class Gravity :public Interaction
	{
	public:
		Gravity(float gx, float gy, float gz)
			: m_gx(gx), m_gy(gy), m_gz(gz)
		{}

		virtual std::vector<Constraint> comfigure_constraints(std::shared_ptr<Simulation_Data> sim_data) override;

	private:
		float m_gx;
		float m_gy;
		float m_gz;
	};



}