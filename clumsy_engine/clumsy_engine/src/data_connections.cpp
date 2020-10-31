
#include "Simulator.h"
#include "spring_stretch.h"
#include "gravity.h"
#include <functional>

namespace clumsy_engine
{
	void Data_Connections::setup_data_connections(Simulator* sim, Spring_Stretch* iteraction)
	{
		auto s = std::bind(&Spring_Stretch::set_positions, iteraction, std::placeholders::_1);
		sim->m_data_map.get<Position>()->send_to(s);

//		auto s = std::bind(&Spring_Stretch::set_positions, iteraction, std::placeholders::_1);
//		sim->m_sim_data->m_positions.send_to(s);
	}

	void Data_Connections::setup_data_connections(Simulator* sim, Inertial* iteraction)
	{

	}


	void Data_Connections::setup_data_connections(Simulator* sim, Gravity* iteraction)
	{

	}

}
