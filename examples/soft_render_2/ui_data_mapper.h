
#pragma once

#include "Spinning_Cube.h"
#include "Imgui_Wrapper.h"

#include "type_map.h"
#include "member_extractor.h"


namespace soft_render
{


	struct add_all_ui
	{

#define ADD_UI(name,ui_type, extra_type,...) ADD_RELATION_PAIR_RECORD(name, Spinning_Cube::config::name, ui_type, EVAL(soft_render::type_map::variable < extra_type, extra_type{ __VA_ARGS__ }>));
		struct config
		{
			ADD_UI(angle_rate, slider_bar_float3, range, 0.0f, 1.f);
			ADD_UI(lookat, slider_bar_float3, range, -1e3f, 1e3f);
			ADD_UI(camara_location, slider_bar_float3, range, -1e3f, 1e3f);
			ADD_UI(cube_side, slider_bar_float, range, 1e1f, 1e3f);
			ADD_UI(cube_unit, slider_bar_float, range, 0.8f, 10.f);
		};

		struct add_ui
		{
			template<typename rec>
			static void apply(Spinning_Cube& sc, Imgui_Wrapper& ui)
			{
				using tl = rec::type;
				using data_tag = get_nth_element_t<tl, 0>;
				using ui_component = get_nth_element_t<tl, 1>;
				using extra_data_with_default = get_nth_element_t<tl, 2>;
				auto& v = sc.get_config<data_tag>();

				std::string name = typeid(data_tag).name();
				auto ui_component_obj = std::make_shared<ui_component>();

				ui_component_obj->value = &v;
				ui_component_obj->name = name;
				ui_component_obj->extra = extra_data_with_default::get_default_value();

				ui.add_ui_component(ui_component_obj);
			}
		};

		static void apply(Spinning_Cube& sc, Imgui_Wrapper& ui)
		{
			for_each_type<extract_member_type_list_t<config>>::apply<add_ui>(sc, ui);
		}

	};

}