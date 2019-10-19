



#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <memory>

#include "Simulation_Data.h"

class Simulator {
public:
	virtual void update(std::vector<float>& positions,std::vector<int>& triangle_indices)=0;


public:
	virtual void setGravity(float g)=0;

};


#endif