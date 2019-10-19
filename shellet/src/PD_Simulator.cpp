
#include "PD_Simulator.h"

#include <cmath>

void PD_Simulator::update(std::vector<float>& positions,std::vector<int>& triangle_indices) {

	int vNum = positions.size() / 3;
	static float t= 0;
	float a = 1e-2;
	for (int i = 0; i < vNum; i++) {

		positions[i * 3 + 0] += a*std::sin(t);
	}
	t += 1e-1;


}


void PD_Simulator::setGravity(float g) {
	simulation_data->gravity_acceleration = g;
}

void PD_Simulator::init(std::vector<float>& positions, std::vector<int>& triangle_indices) {


}

void PD_Simulator::computeEdge() {

} 