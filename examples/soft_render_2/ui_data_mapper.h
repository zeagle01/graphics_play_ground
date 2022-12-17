
#pragma once

#include "Spinning_Cube.h"
#include "Imgui_Wrapper.h"

#include "type_map.h"
#include "member_extractor.h"
//#include "macro_loop.h"


namespace soft_render
{

	struct ui_data_map
	{
		ADD_RELATION_PAIR_RECORD(angle_rate, config::angle_rate, slider_bar_float3);
		ADD_RELATION_PAIR_RECORD(lookat, config::lookat, slider_bar_float3);
	};


	struct  add_ui
	{
		template<typename rec>
		static void apply(Spinning_Cube& sc, Imgui_Wrapper& ui)
		{
			using data_tag = rec::t0;
			using ui_component = rec::t1;
			vec3& v = sc.get_config<data_tag>();

			printf(" add %s %s\n", typeid(data_tag).name(), typeid(ui_component).name());
			ui.add_ui_component<ui_component>(typeid(data_tag).name(), v);
		}

	};

	static void add_all_ui_components(Spinning_Cube& sc, Imgui_Wrapper& ui)
	{
		for_each_type<extract_member_type_list_t<ui_data_map>>::apply<add_ui>(sc, ui);
	}

}