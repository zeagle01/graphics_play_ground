
#ifndef FEM_SIMULATOR_H
#define FEM_SIMULATOR_H

#include "Simulator.h"
#include "Topology_Computer.h"

#include "predef_types.h"

#include <vector>
#include <memory>

class FEM_Simulator :public Simulator {
public:
	FEM_Simulator() : topology_computer(std::make_shared<Topology_Computer>())
	{}


	virtual void update(std::vector<float>& positions,std::vector<int>& triangle_indices);


public:

	virtual void setMesh(const std::vector<float>& positions,const std::vector<int>& triangle_indices) override;
	virtual void setGravity(const std::vector<float>& g)override;
	virtual void setMass(const float mass)override;
	virtual void setDeltaT(const float dt)override;

 


private:
	std::shared_ptr<Topology_Computer> topology_computer;

	m3xf m_forces ;
	m3xf m_velocities ;
	m3xf m_positions ;
	m2xf m_init_positions ;
	std::vector<int> m_indices;

	float dt;

public:
	void compute_new_stretched_positions( std::vector<float>& x);
};


#endif