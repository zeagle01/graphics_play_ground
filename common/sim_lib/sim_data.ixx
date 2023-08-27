
module;

#include "clumsy_lib/field_defines.h" 
#include <vector> 
#include <array>

export module sim_lib: sim_data;


namespace sim_lib
{

	export using float3 = std::array<float, 3>;
	export using int3 = std::array<int, 3>;

	export struct stretch_data
	{
		int triangle_index;
		float stiff;
	};

	export struct bending_data
	{
		int v0,v1;
		float stiff;
		float rest_angle;
	};

	export struct self_collision_data
	{
		int triangle_index;
		float stiff;
		float thickness;
	};

	export struct obstacle_data
	{
		float stiff;
		float thickness;
		float3 velocity;
	};

	export enum class solver_type
	{
		Dummy,
		Newton,
		Explicit,
	};

	export struct sim_data
	{
		CE_ADD_NODE(solver, CE_TYPE(solver_type));

		CE_ADD_NODE(positions, CE_TYPE(std::vector<float3>));
		CE_ADD_NODE(triangles, CE_TYPE(std::vector<int3>));

		CE_ADD_NODE(obstacle_vert_index, CE_TYPE(std::vector<int>));

		CE_ADD_NODE(stretch, CE_TYPE(std::vector<stretch_data>));

		CE_ADD_NODE(bending, CE_TYPE(std::vector<bending_data>));

		CE_ADD_NODE(self_collision, CE_TYPE(std::vector<self_collision_data>));


	};

}