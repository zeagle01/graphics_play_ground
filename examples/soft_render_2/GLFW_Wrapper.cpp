
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
		glfwSetWindowUserPointer(m_window, &m_call_back);

		return window;
	}

	void GLFW_Wrapper::main_loop(std::function<void()> fn)
	{

		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();

			fn();

			glfwMakeContextCurrent(m_window); //in case for docking gui

			glfwSwapBuffers(m_window);
		}

	}

	void GLFW_Wrapper::add_click_fn(std::function<void()> pressed_fn, std::function<void()> release_fn)
	{
		m_call_back.press_fn = pressed_fn;
		m_call_back.release_fn = release_fn;

		glfwSetMouseButtonCallback(m_window, 
			[](GLFWwindow* window, int button, int action, int mods)
			{
				auto callbacks = *(call_back*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					callbacks.press_fn();
					break;
				}
				case GLFW_RELEASE:
				{
					callbacks.release_fn();
					break;
				}
				}

			}
		);
	}

}
