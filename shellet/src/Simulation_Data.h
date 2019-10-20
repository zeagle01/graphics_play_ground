



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