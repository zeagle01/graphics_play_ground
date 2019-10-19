
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
	virtual void setGravity(float g)override;
 

public:
	void init(std::vector<float>& positions,std::vector<int>& triangle_indices);

	void computeEdge();

private:
	std::shared_ptr<Simulation_Data> simulation_data;

};


#endif