
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
		ADD_RELATION_PAIR_RECORD(angle_rate, config::angle_rate, slider_bar_float3,soft_render::range<0.f,1.f>);
		ADD_RELATION_PAIR_RECORD(lookat, config::lookat, slider_bar_float3, soft_render::range<-1e3f, 1e3f>);
		ADD_RELATION_PAIR_RECORD(camara_location, config::camara_location, slider_bar_float3, soft_render::range<-1e3f, 1e3f>);
	};


	struct  add_ui
	{
		template<typename rec>
		static void apply(Spinning_Cube& sc, Imgui_Wrapper& ui)
		{
			using tl = rec::type;
			using data_tag = get_nth_element_t<tl, 0>;
			using ui_component = get_nth_element_t<tl, 1>;
			using rg = get_nth_element_t<tl, 2>;
			vec3& v = sc.get_config<data_tag>();

			std::string name = typeid(data_tag).name();
			ui.add_ui_component<ui_component, data_tag::type >(name, v, rg::min, rg::max);
		}

	};

	static void add_all_ui_components(Spinning_Cube& sc, Imgui_Wrapper& ui)
	{
		for_each_type<extract_member_type_list_t<ui_data_map>>::apply<add_ui>(sc, ui);
	}

}