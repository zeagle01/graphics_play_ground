

#pragma once

namespace clumsy_engine
{
	class Simulator;
	class Spring_Stretch;
	class Inertial;
	class Gravity;

	class Data_Connections
	{
	public:
		static void setup_data_connections(Simulator* sim, Spring_Stretch* iteraction);
		static void setup_data_connections(Simulator* sim, Inertial* iteraction);
		static void setup_data_connections(Simulator* sim, Gravity* iteraction);
	};

}
