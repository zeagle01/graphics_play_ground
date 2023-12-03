

#include "mouse_picker.h"
#include <iostream>
#include "matrix_math/matrix_math.h"

import geometry_lib;

int mouse_picker:: pick(int x,int y)
{

	printf(" %d %d \n", x, y);
	geometry::bvh bvh;

	std::vector<vec3f> rep_points;
	bvh.build(rep_points);

	geometry::bvh_traverser tr;

	return 0;
}

