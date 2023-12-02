
module;

#include "clumsy_lib/type_list.h"
#include "clumsy_lib/field_defines.h"
#include "clumsy_lib/macro_loop.h"

#include <vector>

module sim_lib:simulator_datas;

import: small_mat;
import: simulator_data_update;
import : sim_data;

import :struct_functors;

namespace sim_lib
{

	using namespace simulator_data_update;

#define CE_SIMULATOR_DATA(name,type,update_fn,dep_list,tag_list) CE_ADD_NODE(name, CE_TYPE(type) CE_FIELD(dep_update_fn,update_fn) CE_FIELD(deps,EVAL(dep_list)) CE_FIELD(tags,EVAL(tag_list)))

	template<typename ...T> using tl = clumsy_lib::type_list<T...>;

	struct simulator_datas
	{
		//geometry
		CE_SIMULATOR_DATA(vert_size,		int,					assign, EVAL(tl<sim_data::vertex_num>), tl<>);
		CE_SIMULATOR_DATA(positions,		std::vector<vec3>,		convert_pos, EVAL(tl<sim_data::positions >), tl<>);

		CE_SIMULATOR_DATA(dynamic_verts,	std::vector<int>,		compute_complete_set, EVAL(tl<sim_data::vertex_num,sim_data::obstacle_vert_index>), tl<>);
		CE_SIMULATOR_DATA(fixed_verts,		std::vector<int>,		assign, EVAL(tl<sim_data::obstacle_vert_index>), tl<>);


		CE_SIMULATOR_DATA(stretch_edges,	std::vector<int2>,		assign, EVAL(tl<sim_data::stretch_edges >), tl<>);

		CE_SIMULATOR_DATA(gravity,			vec3,					assign, EVAL(tl<sim_data::gravity >), tl<>);
		CE_SIMULATOR_DATA(time_step,		float,					assign, EVAL(tl<sim_data::time_step >), tl<>);
		CE_SIMULATOR_DATA(density,			float,					assign, EVAL(tl<sim_data::density >), tl<>);

		//dynamic derived
		CE_SIMULATOR_DATA(edge_lengths,		std::vector<float>,		compute_edge_lengths, EVAL(tl<stretch_edges,positions >), tl<>);

	};

}
