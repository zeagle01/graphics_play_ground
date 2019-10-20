
#include "PD_Simulator.h"

#include <cmath>
#include "Eigen/Dense"

//public:
void PD_Simulator::update(std::vector<float>& positions,std::vector<int>& triangle_indices) {

	int vNum = positions.size()/3;
	store_last_positions(simulation_data->last_positions_NEW, simulation_data->positions_NEW);
	external_force_init(simulation_data->external_forces_NEW,vNum);
	apply_gravity( simulation_data->external_forces_NEW, simulation_data-> mass_NEW, simulation_data-> gravity_acceleration_NEW);


	//std::vector<Eigen::Vector3f> S(positions.size()/3);
	if (simulation_data->velocities_NEW.empty()) {
		simulation_data->velocities_NEW.resize(vNum,Eigen::Vector3f::Zero());
	}
	compute_guess_positions(simulation_data->positions_NEW, simulation_data->last_positions_NEW, simulation_data->velocities_NEW ,  simulation_data->mass_NEW,simulation_data->external_forces_NEW, simulation_data->dt);

	for (int i = 0; i < vNum; i++) {
		positions[i * 3 + 0] = simulation_data->positions_NEW[i][0];
		positions[i * 3 + 1] = simulation_data->positions_NEW[i][1];
		positions[i * 3 + 2] = simulation_data->positions_NEW[i][2];
	}
	update_velocities(simulation_data->velocities_NEW, simulation_data->positions_NEW, simulation_data->last_positions_NEW, simulation_data->dt);

	



}


// protected:
void PD_Simulator::external_force_init(std::vector<Eigen::Vector3f>& external_forces, const int vertexNum) {
	external_forces.resize(vertexNum ,Eigen::Vector3f::Zero());
}
void PD_Simulator::apply_gravity(std::vector<Eigen::Vector3f>& external_forces, const Eigen::VectorXf& mass, const Eigen::Vector3f& gravity_acceleration) {
	for (int i = 0; i < mass.size(); i++) {
		external_forces[i] = mass[i] * gravity_acceleration;
	}
}
void PD_Simulator::compute_guess_positions(std::vector<Eigen::Vector3f>& guess_X, const std::vector<Eigen::Vector3f>& X, const std::vector<Eigen::Vector3f>& V, const Eigen::VectorXf& mass, const std::vector<Eigen::Vector3f>& external_forces, const float dt) {

	for (int i = 0; i < X.size() ; i++) {
		guess_X[i ] = X[i ] + dt * V[i ] + dt * dt * external_forces[i ] / mass[i];
	}
}
void PD_Simulator::store_last_positions(std::vector<Eigen::Vector3f>& last_X, std::vector<Eigen::Vector3f>& X) {
	last_X = X;
}

void PD_Simulator::update_velocities(std::vector<Eigen::Vector3f>& velocities, const  std::vector<Eigen::Vector3f>& X, const std::vector<Eigen::Vector3f>& X0, const float dt) {

	for (int i = 0; i < X.size(); i++) {
		velocities[i] = (X[i] - X0[i]) / dt;
	}
}




//setters:
void PD_Simulator::setMesh(const std::vector<float>& positions, const std::vector<int>& triangle_indices) {
	int vNum = positions.size() / 3;
	for (int i = 0; i < vNum; i++) {
		simulation_data->positions_NEW.push_back( Eigen::Vector3f(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]));

	}
	int tNum = triangle_indices.size()/3;
	for (int i = 0; i < tNum; i++) {
		simulation_data->triangle_indices_NEW.push_back(  Eigen::Vector3i(triangle_indices[i * 3 + 0], triangle_indices[i * 3 + 1], triangle_indices[i * 3 + 2]));

	}

}

void PD_Simulator::setGravity(const std::vector<float>& g) {

	simulation_data->gravity_acceleration_NEW = Eigen::Vector3f(g[0], g[1], g[2]);


}

void PD_Simulator::setMass(const float mass) {

	int vNum = simulation_data->positions_NEW.size() ;
	simulation_data->mass_NEW = Eigen::VectorXf::Constant(vNum,  mass);

}

 void PD_Simulator::setDeltaT(const float dt) {
	 simulation_data->dt = dt;
 }

void PD_Simulator::init(std::vector<float>& positions, std::vector<int>& triangle_indices) {


}

void PD_Simulator::computeEdge() {

} 