
#pragma once

#include <array>


struct picked_info
{
	int t_index;
	std::array<float, 3> bary;
};

class mouse_picker
{
public:
	picked_info pick(
		const std::array<float, 3>& p, const std::array<float, 3>& dir,
		const float* x, const int* indices, int vNum, int tNum
	);

};