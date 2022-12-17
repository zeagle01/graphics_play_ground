
#pragma once

#include <functional>
#include <vector>
#include "mat.h"
#include "type_map.h"


struct GLFWwindow;

namespace soft_render
{

	struct slider_bar_float3
	{
		vec3* value;
		float min = 0.f;
		float max = 1.f;
		std::string name = "aa";

		void operator()();
	};

	struct gui_component
	{
		ADD_MEMBER_POINTER(slider_bar_float3, soft_render::slider_bar_float3);
	};


	class Imgui_Wrapper
	{

	public:
		void init(GLFWwindow* window);
		void update();

		template<typename ui,typename value_t>
		void add_ui_component(value_t& value)
		{
			m_ui_components.add_type<ui>(&value);
		}

	private:
		type_map m_ui_components;
	};
}
