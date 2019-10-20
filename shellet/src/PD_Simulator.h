
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


	void external_force_init(a3f& external_forces,const int vertexNum);
	void apply_gravity(a3f& external_forces, const vXf& mass, const v3f& gravity_acceleration );
	void compute_guess_positions(a3f& guess_X, const a3f& X,const a3f& V, const vXf& mass, const a3f& external_forces, const float dt);
	void store_last_positions(a3f& last_X, const a3f& X);
	void update_velocities(a3f& velocities,const  a3f& X,const a3f& X0,const float dt);

private:
	std::shared_ptr<Simulation_Data> simulation_data;

};


#endif