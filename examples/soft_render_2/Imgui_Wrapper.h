
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
		float min = 0.f;
		float max = 1.f;
		std::string name = "aa";

		void operator()();
	};

	struct gui_component
	{
		struct slider_bar_float3 : type_map::variable<soft_render::ui_slider_bar_float3> {} ;
		slider_bar_float3* aa;
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

		struct call
		{
			template<typename T>
			static void apply(type_map& tm)
			{
				auto fn = tm.get_ref<T>();
				fn();
			}
		};

	private:
		type_map m_ui_components;
	};
}
