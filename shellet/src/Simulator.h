



#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <string>


#include "Simulation_Data.h"

class Simulator {
public:
	virtual void update(std::vector<float>& positions,std::vector<int>& triangle_indices)=0;


public:
	virtual void setMesh(const std::vector<float>& positions,const std::vector<int>& triangle_indices)=0;
	virtual void setGravity(const std::vector<float>& g)=0;
	virtual void setMass(const float mass)=0;
	virtual void setDeltaT(const float dt)=0;

public:
	static std::shared_ptr<Simulator> new_instance(std::string type);
public:
	using Simulator_Constructor = std::function<std::shared_ptr<Simulator>()>;
	static std::map<std::string, Simulator_Constructor> m_constructors;
	static void fill_types();
};

//template<typename Base,typename Sub>
//std::shared_ptr<Base> newT
//{
//    auto sub=std::make_shared<Sub>();
//    return std::dynamic_pointer_cast<Base>(sub);
//}

#endif