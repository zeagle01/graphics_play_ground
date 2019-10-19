



#ifndef SIMULATION_DATA_H
#define SIMULATION_DATA_H


#include <vector>
class Simulation_Data {
public:
	std::vector<float> X;
	std::vector<int> TV;
	std::vector<int> EV;

	float gravity_acceleration;
};


#endif