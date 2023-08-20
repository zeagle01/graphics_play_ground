
module;

#include "clumsy_lib/field_defines.h" 
#include <vector> 

export module sim_lib: sim_data;


namespace sim_lib
{

	export struct sim_data
	{
		CE_ADD_NODE(positions, CE_TYPE(std::vector<float>));
	};

}