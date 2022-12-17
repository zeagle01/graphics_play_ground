
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
	private:
		GLFWwindow* m_window;
	};
}