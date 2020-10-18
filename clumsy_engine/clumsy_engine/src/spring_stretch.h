

#pragma  once

#include "interaction.h"

namespace clumsy_engine
{
	class Spring_Stretch:public Interaction
	{
	public:

		std::vector<Constraint> comfigure_constraints(std::shared_ptr<Simulation_Data> sim_data) override;

	};
}