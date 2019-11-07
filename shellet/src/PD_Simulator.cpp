
#include "PD_Simulator.h"

#include <cmath>
#include "Eigen/Eigen"




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


	if (simulation_data->edge_topology_changed) {
		topology_computer->precompute( simulation_data->triangle_indices);
		topology_computer->get_edge_indices(simulation_data->edge_indices);

		//
		int e_num=simulation_data->edge_indices.cols();
		simulation_data->edge_rest_length.resize(e_num);
		for (int ei = 0; ei < e_num; ei++) {
			float w[]{ 1,-1 };
			v2i ev= simulation_data->edge_indices.col(ei);
			v3f eX[] = { simulation_data->positions.col(ev[0]),simulation_data->positions.col(ev[1]) };
			v3f d = w[0] * eX[0] + w[1] * eX[1];
			simulation_data->edge_rest_length[ei] = d.norm();
		}

		simulation_data->edge_topology_changed = false;
	}

	float spring_k = 1e4;
	m3xf b0;
	m3xf b;
	smf A;
	{
		static bool first = true;
		if (first) {
			using eigen_triplet = Eigen::Triplet<double>;
			std::vector<eigen_triplet > coefficients;
			A=smf(vNum, vNum);
			float dt = simulation_data->dt;
			for (int i = 0; i < vNum; i++) {
				float mass_i=simulation_data->mass[i];
				coefficients.push_back(eigen_triplet(i, i, mass_i/dt/dt));
			}

			for (int ei = 0; ei < simulation_data->edge_indices.cols(); ei++) {
				v2i ev = simulation_data->edge_indices.col(ei);
				coefficients.push_back(eigen_triplet(ev[0], ev[0], spring_k));
				coefficients.push_back(eigen_triplet(ev[0], ev[1], -spring_k));
				coefficients.push_back(eigen_triplet(ev[1], ev[1], spring_k));
				coefficients.push_back(eigen_triplet(ev[1], ev[0], -spring_k));
			}

			A.setFromTriplets(coefficients.begin(), coefficients.end());
			b0 = simulation_data->mass.asDiagonal() * simulation_data->positions / dt / dt;

		}
		//first = false;
	}

	//a3xf ff;
	


	Eigen::SimplicialCholesky<smf> chol(A);  // performs a Cholesky factorization of A

	for (int it = 0; it < 20; it++) {

		b = b0;
		//edge constraits
		for (int ei = 0; ei < simulation_data->edge_indices.cols(); ei++) {
			float w[]{ 1,-1 };
			v2i ev= simulation_data->edge_indices.col(ei);
			v3f eX[] = { simulation_data->positions.col(ev[0]),simulation_data->positions.col(ev[1]) };
			v3f predicte_p = w[0] * eX[0] + w[1] * eX[1];
			float l = predicte_p.norm();
			predicte_p = predicte_p / l * simulation_data->edge_rest_length[ei];
			b.col(ev[0]) += w[0]*predicte_p*spring_k;
			b.col(ev[1]) += w[1]*predicte_p*spring_k;
		}
		simulation_data->positions.row(0)  = chol.solve(b.transpose().col(0));
		simulation_data->positions.row(1)  = chol.solve(b.transpose().col(1));
		simulation_data->positions.row(2)  = chol.solve(b.transpose().col(2));
	}

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
	//int tNum=
	//std::sort(face_indices.col(0),face_indices.col())


}

void PD_Simulator::compute_edge_constraints() {
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