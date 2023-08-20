
module;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

module quick_shell:ui_component_imp;

import : ui_components;

namespace quick_shell
{
	template<typename ui_com>
	class ui_component_imp;

	template< typename ui_com>
	struct value_and_extra_data
	{
		std::string name;
		typename ui_com::type* value;
		typename ui_com::extra_data extra;
	};


#define DEF_UI_COMPONENT_IMP(ui_com,...)									\
template<> struct ui_component_imp<ui_com> : value_and_extra_data<ui_com>	\
{																			\
	bool operator()()														\
	{																		\
		__VA_ARGS__															\
	}																		\
};																			\



DEF_UI_COMPONENT_IMP (ui_component::slider_bar,
	return ImGui::SliderFloat(name.c_str(), value, extra.min, extra.max);
)

DEF_UI_COMPONENT_IMP (ui_component::slider_bar2,
	return ImGui::SliderFloat2(name.c_str(), value->data(), extra.min, extra.max);
)

DEF_UI_COMPONENT_IMP (ui_component::slider_bar3,
	return ImGui::SliderFloat3(name.c_str(), value->data(), extra.min, extra.max);
)


DEF_UI_COMPONENT_IMP(ui_component::check_box,
	return ImGui::Checkbox(name.c_str(), value);
)

DEF_UI_COMPONENT_IMP(ui_component::text_line,
	auto str = name + ": " + *value;
	ImGui::Text(str.c_str());
	return false;
)


#undef DEF_UI_COMPONENT_IMP

}
