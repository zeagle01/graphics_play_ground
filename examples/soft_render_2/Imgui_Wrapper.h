
#pragma once

#include <functional>
#include <vector>
#include "mat.h"


struct GLFWwindow;

namespace soft_render
{
	class Imgui_Wrapper
	{
		using get_value_t = std::function<float& ()>;
		using get_vec3_t = std::function<vec3& ()>;
	public:
		void init(GLFWwindow* window);
		void update();

		void add_slider_bar(get_value_t get_value);
		void add_slider_bar_float3(get_vec3_t get_value);
		//void add_button();

	private:
		std::vector<get_value_t> m_sliders;
		std::vector<get_vec3_t> m_sliders_float3;
	};
}
