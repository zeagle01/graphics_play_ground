
#include "PD_Simulator.h"

#include <cmath>
#include "Eigen/Dense"

//public:
void PD_Simulator::update(std::vector<float>& positions,std::vector<int>& triangle_indices) {

	int vNum = positions.size()/3;
	store_last_positions(simulation_data->last_positions, simulation_data->positions);


	//store_last_positions(simulation_data->last_positions, simulation_data->positions);
	external_force_init(simulation_data->external_forces,vNum);
	apply_gravity( simulation_data->external_forces, simulation_data-> mass, simulation_data-> gravity_acceleration);


	//std::vector<Eigen::Vector3f> S(positions.size()/3);
	if (simulation_data->velocities.empty()) {
		simulation_data->velocities.resize(vNum,Eigen::Vector3f::Zero());
	}
	compute_guess_positions(simulation_data->positions, simulation_data->last_positions, simulation_data->velocities ,  simulation_data->mass,simulation_data->external_forces, simulation_data->dt);

	//positions = std::vector<float>(simulation_data->positions_NEW[0][0], simulation_data->positions_NEW[0][0] + vNum * 3);
	positions = simulation_data->positions.convert_2_std_vector<float>();

	update_velocities(simulation_data->velocities, simulation_data->positions, simulation_data->last_positions, simulation_data->dt);

	



}


// protected:
void PD_Simulator::external_force_init(a3f& external_forces, const int vertexNum) {

	external_forces.resize(vertexNum ,Eigen::Vector3f::Zero());

}
void PD_Simulator::apply_gravity(a3f& external_forces, const vXf& mass, const v3f& gravity_acceleration) {

	for (int i = 0; i < mass.size(); i++) {
		external_forces[i] = mass[i] * gravity_acceleration;
	}

}
void PD_Simulator::compute_guess_positions(a3f& guess_X, const a3f& X, const a3f& V, const vXf& mass, const a3f& external_forces, const float dt) {
	for (int i = 0; i < X.size() ; i++) {
		guess_X[i ] = X[i ] + dt * V[i ] + dt * dt * external_forces[i ] / mass[i];
	}

}
void PD_Simulator::store_last_positions(a3f& last_X, const a3f& X) {
	last_X = X;

}
void PD_Simulator::update_velocities(a3f& velocities, const  a3f& X, const a3f& X0, const float dt) {
	for (int i = 0; i < X.size(); i++) {
		velocities[i] = (X[i] - X0[i]) / dt;
	}
}



//setters:
void PD_Simulator::setMesh(const std::vector<float>& positions, const std::vector<int>& triangle_indices) {
	simulation_data->positions.assignFromContinuousStorage( &positions[0],positions.size());
	simulation_data->triangle_indices.assignFromContinuousStorage(&triangle_indices[0], triangle_indices.size());

}

void PD_Simulator::setGravity(const std::vector<float>& g) {


	simulation_data->gravity_acceleration = v3f(g[0], g[1], g[2]);


}

void PD_Simulator::setMass(const float mass) {

	int vNum = simulation_data->positions.size() ;
	simulation_data->mass = vXf::Constant(vNum,  mass);

}

 void PD_Simulator::setDeltaT(const float dt) {
	 simulation_data->dt = dt;
 }

void PD_Simulator::init(std::vector<float>& positions, std::vector<int>& triangle_indices) {


}

void PD_Simulator::computeEdge() {

} 