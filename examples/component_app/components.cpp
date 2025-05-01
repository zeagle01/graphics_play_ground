
module;

//#include <span>
#include <memory>

module app:components;

import :matrix_def;


struct position
{
	vec3f data;
};

struct position_buffer
{
	std::shared_ptr<vec3f[]> data;
	int num;
};

struct triangle_index_buffer
{
	std::shared_ptr<vec3i[]> data;
	int num;
};
