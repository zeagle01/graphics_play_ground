
#ifndef PD_SIMULATOR_H
#define PD_SIMULATOR_H

#include "Simulator.h"
#include "Simulation_Data.h"

#include <vector>
#include <memory>

class PD_Simulator :public Simulator {
public:
	virtual void update(std::vector<float>& positions,std::vector<int>& triangle_indices);
	PD_Simulator() :simulation_data(std::make_shared<Simulation_Data>()) {}


public:

	virtual void setMesh(const std::vector<float>& positions,const std::vector<int>& triangle_indices);
	virtual void setGravity(const std::vector<float>& g)override;
	virtual void setMass(const float mass)override;
	virtual void setDeltaT(const float dt)override;

 

public:
	void init(std::vector<float>& positions,std::vector<int>& triangle_indices);

	void computeEdge();

protected:
	void external_force_init(std::vector<float>& external_forces,const int vertexNum);
	void apply_gravity(std::vector<float>& external_forces, const std::vector<float>& mass, const std::vector<float> gravity_acceleration );
	void compute_guess_positions(std::vector<float>& guess_X, const std::vector<float>& X,const std::vector<float>& V, const std::vector<float>& mass, const std::vector<float>& external_forces, const float dt);
	void store_last_positions(std::vector<float>& last_X, std::vector<float>& X);
	void update_velocities(std::vector<float>& velocities,const  std::vector<float>& X,const std::vector<float>& X0,const float dt);

	void external_force_init(std::vector<Eigen::Vector3f>& external_forces,const int vertexNum);
	void apply_gravity(std::vector<Eigen::Vector3f>& external_forces, const Eigen::VectorXf& mass, const Eigen::Vector3f& gravity_acceleration );
	void compute_guess_positions(std::vector<Eigen::Vector3f>& guess_X, const std::vector<Eigen::Vector3f>& X,const std::vector<Eigen::Vector3f>& V, const Eigen::VectorXf& mass, const std::vector<Eigen::Vector3f>& external_forces, const float dt);
	void store_last_positions(std::vector<Eigen::Vector3f>& last_X, std::vector<Eigen::Vector3f>& X);
	void update_velocities(std::vector<Eigen::Vector3f>& velocities,const  std::vector<Eigen::Vector3f>& X,const std::vector<Eigen::Vector3f>& X0,const float dt);

private:
	std::shared_ptr<Simulation_Data> simulation_data;

};


#endif