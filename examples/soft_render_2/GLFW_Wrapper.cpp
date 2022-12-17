
#include "GLFW_Wrapper.h"

#include <iostream>

namespace soft_render
{

	GLFWwindow* GLFW_Wrapper::create_window(int width, int height)
	{
		auto status = glfwInit();
		if (status == GLFW_TRUE)
		{
			printf("glfw init succeed! \n");
		}
		else
		{
			printf("glfw init error! \n");
		}

		GLFWwindow* window;
		window = glfwCreateWindow(width, height, "gl window", nullptr, nullptr);

		if (window)
		{
			printf(" window %p is created\n", window);
		}
		else
		{
			printf(" window can't be created\n");
		}

		glfwMakeContextCurrent(window);

		glfwSwapInterval(1);

		m_window = window;
		return window;
	}

	void GLFW_Wrapper::main_loop(std::function<void()> fn)
	{

		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();

			fn();

			glfwSwapBuffers(m_window);
		}

	}
}
