
#pragma once

#include "GLFW/glfw3.h"
#include <functional>

struct GLFWwindow;

namespace soft_render
{

	class GLFW_Wrapper
	{
	public:
		GLFWwindow* create_window(int width, int height);
		void main_loop(std::function<void()>);

		void add_click_fn(std::function<void()> pressed_fn, std::function<void()> release_fn);
	private:
		GLFWwindow* m_window;

		struct call_back
		{
			std::function<void()> press_fn;
			std::function<void()> release_fn;
		} m_call_back;
	};
}