

#include "FEM_Simulator.h"

#include <algorithm>


void FEM_Simulator::update(std::vector<float>& positions, std::vector<int>& triangle_indices) 
{

    //TODO
    //implicit matrix assemble

//    static float time = 0.0;
//    //solve
//    for(size_t i=0;i<positions.size();i++)
//    {
//        positions[i%3+1]+=std::sin(time);
//    }
//    time += 0.001;
}



void FEM_Simulator::setMesh(const std::vector<float>& positions, const std::vector<int>& triangle_indices) {

}
void FEM_Simulator::setGravity(const std::vector<float>& g) {

}
void FEM_Simulator::setMass(const float mass) {

}
void FEM_Simulator::setDeltaT(const float dt) {

}
