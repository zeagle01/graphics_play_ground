



#include "simulator.h"
#include "interaction.h"


namespace clumsy_engine
{
	void Simulator::add_interaction(std::unique_ptr<Interaction> interaction)
	{
		//m_interations.push_back(std::move(interaction));
	}

	const std::vector<float>& Simulator::get_delta_positions()const
	{
		return m_delta_positions;
	}

}