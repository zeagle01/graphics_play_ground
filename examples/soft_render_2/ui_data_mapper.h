
#pragma once

#include "Spinning_Cube.h"
#include "Imgui_Wrapper.h"

#include "type_map.h"
#include "member_extractor.h"


namespace soft_render
{

	struct add_all_ui
	{
#define ADD_UI_RECORD(obj_field,obj,ui_type, ui_extra,...) ADD_RELATION_PAIR_RECORD(obj_field, obj, obj##::##config##::##obj_field, ui_type, soft_render::type_map::variable < ui_extra, ui_extra{ __VA_ARGS__ } > )

		struct config
		{
			ADD_UI_RECORD(angle_rate,			Spinning_Cube,			slider_bar_float3,			 range, 0.0f, 1.f);
			ADD_UI_RECORD(cube_side,			Spinning_Cube,			slider_bar_float,			 range, 1e1f, 1e3f);
			ADD_UI_RECORD(cube_unit,			Spinning_Cube,			slider_bar_float,			 range, 0.8f, 10.f);
			ADD_UI_RECORD(screen_size,			Spinning_Cube_App,		slider_bar_float2,			 range, 100.f, 1000.f);
		};

		struct add_ui
		{
			template<typename rec>
			static void apply(type_map_view tm, Imgui_Wrapper& ui)
			{
				using tl = rec::type;

				using obj_t = get_nth_element_t<tl, 0>;
				using obj_field_t = get_nth_element_t<tl, 1>;
				using ui_component_t = get_nth_element_t<tl, 2>;
				using ui_extra_data_t = get_nth_element_t<tl, 3>;

				auto& obj = tm.get_ref<obj_t>();
				auto& v = obj.m_configs.get_ref<obj_field_t>();

				std::string name = extract_name(typeid(obj_field_t).name());
				auto ui_component_obj = std::make_shared<ui_component_t>();

				ui_component_obj->value = &v;
				ui_component_obj->name = name;
				ui_component_obj->extra = ui_extra_data_t::get_default_value();

				ui.add_ui_component(ui_component_obj);
			}
		};

		static void apply(type_map_view tm, Imgui_Wrapper& ui)
		{
			for_each_type<extract_member_type_list_t<config>>::apply<add_ui>(tm, ui);
		}
	};

}