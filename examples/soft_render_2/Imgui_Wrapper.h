
#pragma once

#include <functional>
#include <vector>
#include "mat.h"
#include "type_map.h"


struct GLFWwindow;

namespace soft_render
{

	struct no_extra {};

	struct range
	{
		float min;
		float max;
	};


	struct ui_component_base
	{
		std::string name;
		void virtual operator()() = 0;
	};

	template< typename T,typename extra_data> 
	struct value_and_extra_data
	{
		T* value;
		extra_data extra;
	};


	struct slider_bar_float3 :ui_component_base, value_and_extra_data<vec3, range>
	{
		void operator()()override;
	};

	struct slider_bar_float2 :ui_component_base, value_and_extra_data<vec2, range>
	{
		void operator()()override;
	};

	struct slider_bar_float :ui_component_base, value_and_extra_data<float, range>
	{
		void operator()()override;
	};

	struct check_box :ui_component_base, value_and_extra_data<bool, no_extra>
	{
		void operator()()override;
	};



	class Imgui_Wrapper
	{

	public:
		void init(GLFWwindow* window);
		void update();

		void add_ui_component(std::shared_ptr<ui_component_base> ui_component)
		{
			m_ui_components.push_back(ui_component);
		}

	private:
		std::vector<std::shared_ptr<ui_component_base>> m_ui_components;
	};
}
