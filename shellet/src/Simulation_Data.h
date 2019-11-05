



#ifndef SIMULATION_DATA_H
#define SIMULATION_DATA_H


#include <vector>

#include "predef_types.h"

class Simulation_Data {
public:


	m3xf positions;
	m3xi triangle_indices;

	m2xi edge_indices;


	m3xf last_positions;
	m3xf velocities;
	m3xf external_forces;

	vxf mass;
	v3f gravity_acceleration;

	float dt;

	bool edge_topology_changed=true;
};


#endif