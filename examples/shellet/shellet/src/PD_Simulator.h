
#ifndef PD_SIMULATOR_H
#define PD_SIMULATOR_H

#include "Simulator.h"
#include "Simulation_Data.h"
#include "Topology_Computer.h"

#include <vector>
#include <memory>

class PD_Simulator :public Simulator {
public:
	PD_Simulator() :simulation_data(std::make_shared<Simulation_Data>()),
		topology_computer(std::make_shared<Topology_Computer>())
	{}

	virtual void update(std::vector<float>& positions,std::vector<int>& triangle_indices) override;


public:

	virtual void setMesh(const std::vector<float>& positions,const std::vector<int>& triangle_indices);
	virtual void setGravity(const std::vector<float>& g)override;
	virtual void setMass(const float mass)override;
	virtual void setDeltaT(const float dt)override;

 

public:
	void init(std::vector<float>& positions,std::vector<int>& triangle_indices);

	void computeEdge();

protected:



	void external_force_init(m3xf& external_forces,const int vertexNum);
	void apply_gravity(m3xf& external_forces, const vxf& mass, const v3f& gravity_acceleration );
	void compute_inertial_positions(m3xf& guess_X, const m3xf& X,const m3xf& V, const vxf& mass, const m3xf& external_forces, const float dt);
	void store_last_positions(m3xf& last_X, const m3xf& X);
	void update_velocities(m3xf& velocities,const  m3xf& X,const m3xf& X0,const float dt);

	void compute_edge_constraints();
	void compute_edge_indices(const m3xi& face_indices);

private:
	std::shared_ptr<Simulation_Data> simulation_data;
	std::shared_ptr<Topology_Computer> topology_computer;

};


#endif