
#include "PD_Simulator.h"

#include <cmath>
#include "Eigen/Eigen"
#include <iostream>




//public:
void PD_Simulator::update(std::vector<float>& positions,std::vector<int>& triangle_indices) {

	int vNum = positions.size()/3;
	store_last_positions(simulation_data->last_positions, simulation_data->positions);


	//store_last_positions(simulation_data->last_positions, simulation_data->positions);
	external_force_init(simulation_data->external_forces,vNum);
	apply_gravity( simulation_data->external_forces, simulation_data-> mass, simulation_data-> gravity_acceleration);


	if (!simulation_data->velocities.data()) {
		simulation_data->velocities=m3xf::Zero(3,vNum);
	}
	compute_guess_positions(simulation_data->positions, simulation_data->last_positions, simulation_data->velocities ,  simulation_data->mass,simulation_data->external_forces, simulation_data->dt);

	//compute_edge_indices( simulation_data->triangle_indices);
	compute_edge_constraints();

	positions =std::vector<float>( simulation_data->positions.data(),simulation_data->positions.data()+3*vNum);

	update_velocities(simulation_data->velocities, simulation_data->positions, simulation_data->last_positions, simulation_data->dt);

	



}


// protected:
void PD_Simulator::external_force_init(m3xf& external_forces, const int vertexNum) {
	external_forces = m3xf::Zero(3, vertexNum);
}
void PD_Simulator::apply_gravity(m3xf& external_forces, const vxf& mass, const v3f& gravity_acceleration) {
	external_forces = gravity_acceleration * mass.transpose();
}
void PD_Simulator::compute_guess_positions(m3xf& guess_X, const m3xf& X, const m3xf& V, const vxf& mass, const m3xf& external_forces, const float dt) {
		guess_X = X + dt * V + dt * dt * external_forces * mass.asDiagonal().inverse();

		guess_X.col(0) = v3f(0, 0, 0);
}
void PD_Simulator::store_last_positions(m3xf& last_X, const m3xf& X) {
	last_X = X;
}
void PD_Simulator::update_velocities(m3xf& velocities, const  m3xf& X, const m3xf& X0, const float dt) {
		velocities = (X - X0) / dt;
}


void PD_Simulator::compute_edge_indices(const m3xi& face_indices) {

}

void PD_Simulator::compute_edge_constraints() {

	int vNum = simulation_data->positions.cols();
	using eigen_triplet = Eigen::Triplet<float>;


	bool static first = true;
	if (first) {
		topology_computer->precompute( simulation_data->triangle_indices);
		topology_computer->get_edge_indices(simulation_data->edge_indices);
	}

	int e_num = simulation_data->edge_indices.cols();
	static smf E(vNum, e_num);
	if (first) {
		std::vector<eigen_triplet > E_coefficients;
		for (int ei = 0; ei < e_num; ei++) {
			v2i ev = simulation_data->edge_indices.col(ei);
			float w[]{ 1,-1 };
			E_coefficients.push_back(eigen_triplet( ev[0],ei, w[0]));
			E_coefficients.push_back(eigen_triplet( ev[1],ei, w[1]));
		}
		E.setFromTriplets(E_coefficients.begin(), E_coefficients.end());
	}

	if (first) {
		simulation_data->edge_rest_length = (E.transpose() * simulation_data->positions.transpose()).rowwise().norm();
	}

	float spring_k = 1e4;
	float dt = simulation_data->dt;

	static smf A;
	if (first) {
		A = spring_k * E * E.transpose();
		A += (simulation_data->mass / dt / dt).asDiagonal();
	}
	first = false;



	Eigen::SimplicialCholesky<smf> chol(A);  // performs a Cholesky factorization of A
	mx3f b0 = (simulation_data->mass / dt / dt).asDiagonal() * simulation_data->positions.transpose();
	for (int it = 0; it < 20; it++) {

		mx3f b = b0;
		//edge constraits
		auto p = E.transpose() * simulation_data->positions.transpose();
		auto lengh_p = p.rowwise().norm();

//		std::cout << A << std::endl;
//		std::cout << b << std::endl;

		b += spring_k * E*simulation_data->edge_rest_length.asDiagonal()*lengh_p.asDiagonal().inverse()*p;
		simulation_data->positions.transpose()  = chol.solve(b);
	}

}



//setters:
void PD_Simulator::setMesh(const std::vector<float>& positions, const std::vector<int>& triangle_indices) {
	simulation_data->positions = m3xf::Map(&positions[0], 3, positions.size() / 3);
	simulation_data->triangle_indices = m3xi::Map(&triangle_indices[0], 3, triangle_indices.size() / 3);

}

void PD_Simulator::setGravity(const std::vector<float>& g) {
	simulation_data->gravity_acceleration = v3f(g[0], g[1], g[2]);

}

void PD_Simulator::setMass(const float mass) {

	int vNum = simulation_data->positions.cols() ;
	simulation_data->mass = vxf::Constant(vNum,  mass);

}

 void PD_Simulator::setDeltaT(const float dt) {
	 simulation_data->dt = dt;
 }

void PD_Simulator::init(std::vector<float>& positions, std::vector<int>& triangle_indices) {


}

void PD_Simulator::computeEdge() {

} 