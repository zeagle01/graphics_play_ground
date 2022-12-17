
#pragma once

#include <functional>
#include <vector>


struct GLFWwindow;

class Imgui_Wrapper
{
	using get_value_t = std::function<float& ()>;
public:
	void init(GLFWwindow* window);
	void update();

	void add_slider_bar(get_value_t get_value);
	//void add_button();

private:
	std::vector<get_value_t> m_sliders;
};
