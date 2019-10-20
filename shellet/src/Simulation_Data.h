



#ifndef SIMULATION_DATA_H
#define SIMULATION_DATA_H


#include <vector>

#include "predef_types.h"
#include "Array.hpp"

class Simulation_Data {
public:


	a3f positions;
	a3i triangle_indices;

	a3f last_positions;
	a3f velocities;
	vXf mass;
	a3f external_forces;
	v3f gravity_acceleration;

	float dt;
};


#endif