



#ifndef SIMULATION_DATA_H
#define SIMULATION_DATA_H


#include <vector>

#include "Eigen/Dense"

class Simulation_Data {
public:

	std::vector<Eigen::Vector3f> positions_NEW;
	std::vector<Eigen::Vector3i> triangle_indices_NEW;

	std::vector<Eigen::Vector3f> last_positions_NEW;
	std::vector<Eigen::Vector3f> velocities_NEW;
	Eigen::VectorXf mass_NEW;
	std::vector<Eigen::Vector3f> external_forces_NEW;
	Eigen::Vector3f gravity_acceleration_NEW;

	float dt;
};


#endif