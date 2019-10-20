



#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <memory>


#include "Simulation_Data.h"

class Simulator {
public:
	virtual void update(std::vector<float>& positions,std::vector<int>& triangle_indices)=0;


public:
	virtual void setMesh(const std::vector<float>& positions,const std::vector<int>& triangle_indices)=0;
	virtual void setGravity(const std::vector<float>& g)=0;
	virtual void setMass(const float mass)=0;
	virtual void setDeltaT(const float dt)=0;

};


#endif