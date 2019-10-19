



#ifndef SIMULATION_DATA_H
#define SIMULATION_DATA_H


#include <vector>
class Simulation_Data {
public:
	std::vector<float> X;
	std::vector<int> TV;
	std::vector<int> EV;

	std::vector<float> last_positions;
	std::vector<float> velocities;
	std::vector<float> mass;
	std::vector<float> external_forces;


	std::vector<float> gravity_acceleration;
	float dt;
};


#endif