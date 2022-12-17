
#pragma once

#include <functional>
#include <vector>
#include "mat.h"
#include "type_map.h"


struct GLFWwindow;

namespace soft_render
{

	struct ui_slider_bar_float3
	{
		vec3* value;
		float min=0.f;
		float max=1.f;
		std::string name = "aa";

		void operator()();
	};

	struct gui_component
	{
		struct slider_bar_float3 : type_map::variable<soft_render::ui_slider_bar_float3> {};
	};


	class Imgui_Wrapper
	{
		//using get_value_t = std::function<float& ()>;
		using get_vec3_t = std::function<vec3& ()>;
	public:
		void init(GLFWwindow* window);
		void update();


		template<typename ui,typename value_t>
		void add_ui_component(value_t& value)
		{
			//return m_ui_components.get_ref<var>();
			m_ui_components.add_type<ui>(&value);
		}

	private:
		type_map m_ui_components;
	};
}
