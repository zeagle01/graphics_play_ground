


#pragma once

#include "Interaction.h"
#include "Simulation_Data.h"
#include <vector> 

namespace clumsy_engine
{


	class Inertial :public Interaction
	{
	public:

		Inertial(float time_step) :
			m_time_step(time_step)
		{}

		virtual std::vector<Constraint> comfigure_constraints(std::shared_ptr<Simulation_Data> sim_data) override;


		float get_time_step() {
			return m_time_step;
		}

	private:
		float m_time_step;
	};

}