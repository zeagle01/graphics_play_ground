
module;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "magic_enum/magic_enum.hpp"

#include <string>
#include <functional>

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
		std::function<void(const typename ui_com::type&)> call_back;
	};


#define DEF_UI_COMPONENT_IMP(ui_com,...)									\
template<> struct ui_component_imp<ui_com> : value_and_extra_data<ui_com>	\
{																			\
	bool update_value()														\
	{																		\
		__VA_ARGS__;														\
	}																		\
	void operator()()														\
	{																		\
		if(update_value()&&call_back)										\
		{																	\
			call_back(*value);												\
		}																	\
	}																		\
};																			\



DEF_UI_COMPONENT_IMP (ui_component::slider_bar,
	return ImGui::SliderFloat(name.c_str(), value, extra.min, extra.max);
)

DEF_UI_COMPONENT_IMP (ui_component::slider_bar2,
	return ImGui::SliderFloat2(name.c_str(), value->data(), extra.min, extra.max);
)

DEF_UI_COMPONENT_IMP (ui_component::slider_bar2i,
	return ImGui::SliderInt2(name.c_str(), value->data(), extra.min, extra.max);
)

DEF_UI_COMPONENT_IMP (ui_component::slider_bar3,
	return ImGui::SliderFloat3(name.c_str(), value->data(), extra.min, extra.max);
)

DEF_UI_COMPONENT_IMP (ui_component::input_float,
	return ImGui::InputFloat(name.c_str(), value, 1e-2f, 1e-1f, "%f", ImGuiInputTextFlags_EnterReturnsTrue);
)

DEF_UI_COMPONENT_IMP (ui_component::input_float2,
	return ImGui::InputFloat(name.c_str(), value->data(), 1e-2f, 1e-1f, "%f", ImGuiInputTextFlags_EnterReturnsTrue);
)

DEF_UI_COMPONENT_IMP (ui_component::input_float3,
	return ImGui::InputFloat(name.c_str(), value->data(), 1e-2f, 1e-1f, "%f", ImGuiInputTextFlags_EnterReturnsTrue);
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

namespace quick_shell
{
	template<typename ui_component>
	struct ui_component_imp_new;

	template<> struct ui_component_imp_new<ui_component_new::check_box>
	{
		static bool apply(bool& value, const std::string& name)
		{
			return ImGui::Checkbox(name.c_str(), &value);
		}
	};

	template<> struct ui_component_imp_new<ui_component_new::combo_box>
	{
		//TODO: make magic_enum get_names work
		template<typename enum_type>
		static bool apply(enum_type& value)
		{

			constexpr auto enums = magic_enum::enum_values<enum_type>();
			static char const* c[enums.size()];
			get_c_string_array<enum_type>(c);

			//std::string name = std::string(magic_enum::enum_type_name<enum_type>());
			return ImGui::Combo(__FUNCSIG__, reinterpret_cast<int*>(&value), c, enums.size());
			//return true;
		}
	private:
		template<typename enum_type >
		static void get_c_string_array(char const** c)
		{
			constexpr auto enums = magic_enum::enum_values<enum_type>();
			static std::array<std::string, enums.size()>  str;
			for (int i = 0; i < enums.size(); i++)
			{
				//str[i] = std::string(magic_enum::enum_name(enums[i]));
				str[i] = std::to_string(i);
				c[i] = str[i].c_str();
			}

		}
	};

	template<> struct ui_component_imp_new<ui_component_new::input>
	{
		static bool apply(float& value, const std::string& name)
		{
			return ImGui::InputFloat(name.c_str(), &value, 1e-2f, 1e-1f, "%f", ImGuiInputTextFlags_EnterReturnsTrue);
		}

		static bool apply(int& value, const std::string& name)
		{
			return ImGui::InputInt(name.c_str(), &value, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue);
		}
	};

	//TODO: not working
	//template<> struct ui_component_imp_new<ui_component_new::input3>
	//{
	//	static bool apply(float* value, const std::string& name)
	//	{

	//		return ImGui::InputFloat(name.c_str(), value, 1e-2f, 1e-1f, "%f", ImGuiInputTextFlags_EnterReturnsTrue);
	//	}
	//};

}
