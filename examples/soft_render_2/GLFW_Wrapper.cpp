
#include "GLFW_Wrapper.h"

#include "GLFW/glfw3.h"
#include <iostream>

namespace soft_render
{

	void GLFW_Wrapper::create_window(int width, int height)
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
		glfwSetWindowUserPointer(m_window, &m_dispatcher);
	}

	void GLFW_Wrapper::main_loop(std::function<void(int,int)> fn)
	{

		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();

			double cx;
			double cy;

			glfwGetCursorPos(m_window, &cx, &cy);

			fn(int(cx), int(cy));

			glfwMakeContextCurrent(m_window); //in case for docking gui

			glfwSwapBuffers(m_window);
		}

	}

	void GLFW_Wrapper::setup_input()
	{

		glfwSetWindowSizeCallback(m_window,
			[](GLFWwindow* window, int w, int h)
			{

				auto callbacks = *(dispatcher <Event, bool>*)glfwGetWindowUserPointer(window);
				Window_Resize_Event e;
				e.width = w;
				e.height = h;
				callbacks(e);
			}
		);

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				auto callbacks = *(dispatcher <Event, bool>*)glfwGetWindowUserPointer(window);
				Window_Close_Event e;
				callbacks(e);
			}
		);

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scan_code, int action, int mods)
			{
				auto callbacks = *(dispatcher <Event, bool>*)glfwGetWindowUserPointer(window);

				switch (action)
				{

				case GLFW_PRESS:
				{
					Key_Pressed e;
					e.key_code = key;
					e.repeat_count = 0;
					callbacks(e);
					break;
				}
				case GLFW_REPEAT:
				{
					Key_Pressed e;
					e.key_code = key;
					e.repeat_count = 1;
					callbacks(e);
					break;
				}
				case GLFW_RELEASE:
				{
					Key_Released e;
					e.key_code = key;
					callbacks(e);
					break;
				}

				}
			}
		);

		glfwSetMouseButtonCallback(m_window, 
			[](GLFWwindow* window, int button, int action, int mods)
			{
				auto callbacks = *(dispatcher <Event, bool>*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					Mouse_Pressed e;
					e.button = button;
					callbacks(e);
					break;
				}
				case GLFW_RELEASE:
				{
					Mouse_Released e;
					e.button = button;
					callbacks(e);
					break;
				}
				}

			}
		);

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto callbacks = *(dispatcher <Event, bool>*)glfwGetWindowUserPointer(window);

				Mouse_Scrolled_Event e; (xoffset, yoffset);
				e.dx = xoffset;
				e.dy = yoffset;
				callbacks(e);
			}
		);

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
			{

				auto callbacks = *(dispatcher <Event, bool>*)glfwGetWindowUserPointer(window);

				Mouse_Moved e; (x, y);
				e.x = x;
				e.y = y;
				callbacks(e);

			}
		);
	}

}
