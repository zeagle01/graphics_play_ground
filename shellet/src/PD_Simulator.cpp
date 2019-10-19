
#include "PD_Simulator.h"

#include <cmath>



void PD_Simulator::update_velocities(std::vector<float>& velocities, const  std::vector<float>& X, const std::vector<float>& X0, const float dt) {
	for (int i = 0; i < X.size(); i++) {
		velocities[i] = (X[i] - X0[i]) / dt;
	}

}
void PD_Simulator::store_last_positions(std::vector<float>& last_X, std::vector<float>& X) {
	last_X = X;
}

void PD_Simulator::external_force_init(std::vector<float>& external_forces, const int vertexNum) {
	external_forces.resize(vertexNum*3, 0);
}

void PD_Simulator::apply_gravity(std::vector<float>& external_forces, const std::vector<float>& mass, const std::vector<float> gravity_acceleration ) {
	for (int i = 0; i < mass.size(); i++) {
		external_forces[i * 3 + 0] = mass[i] * gravity_acceleration[0];
		external_forces[i * 3 + 1] = mass[i] * gravity_acceleration[1];
		external_forces[i * 3 + 2] = mass[i] * gravity_acceleration[2];
	}
}

void PD_Simulator::compute_guess_positions(std::vector<float>& guess_X, const std::vector<float>& X,const std::vector<float>& V, const std::vector<float>& mass, const std::vector<float>& external_forces, const float dt) {
	for (int i = 0; i < X.size() / 3; i++) {
		guess_X[i * 3 + 0] = X[i * 3 + 0] + dt * V[i * 3 + 0] + dt * dt * external_forces[i * 3 + 0] / mass[i];
		guess_X[i * 3 + 1] = X[i * 3 + 1] + dt * V[i * 3 + 1] + dt * dt * external_forces[i * 3 + 1] / mass[i];
		guess_X[i * 3 + 2] = X[i * 3 + 2] + dt * V[i * 3 + 2] + dt * dt * external_forces[i * 3 + 2] / mass[i];
	}
}

void PD_Simulator::update(std::vector<float>& positions,std::vector<int>& triangle_indices) {

	store_last_positions(simulation_data->last_positions, positions);
	int vNum = positions.size()/3;
	external_force_init(simulation_data->external_forces,vNum);
	apply_gravity( simulation_data->external_forces, simulation_data-> mass, simulation_data-> gravity_acceleration);


	std::vector<float> S(positions.size());
	if (simulation_data->velocities.empty()) {
		simulation_data->velocities.resize(positions.size());
	}
	compute_guess_positions(S, positions, simulation_data->velocities ,  simulation_data->mass,simulation_data->external_forces, simulation_data->dt);

	positions = S;
	update_velocities(simulation_data->velocities, S, simulation_data->last_positions, simulation_data->dt);
	

//	int eNum=simulation_data->EV.size()/2;
//	for (int i = 0; i < eNum; i++) {
//		
//	}


}

void PD_Simulator::setMesh(const std::vector<float>& positions, const std::vector<int>& triangle_indices) {
	simulation_data->X = positions;
	simulation_data->TV = triangle_indices;
}

void PD_Simulator::setGravity(const std::vector<float>& g) {
	simulation_data->gravity_acceleration = g;
}

void PD_Simulator::setMass(const float mass) {
	int vNum = simulation_data->X.size() / 3;
	simulation_data->mass.resize(vNum);

	for (int i = 0; i < vNum; i++) {
		simulation_data->mass[i] = mass;
	}

}

 void PD_Simulator::setDeltaT(const float dt) {
	 simulation_data->dt = dt;
 }

void PD_Simulator::init(std::vector<float>& positions, std::vector<int>& triangle_indices) {


}

void PD_Simulator::computeEdge() {

} 