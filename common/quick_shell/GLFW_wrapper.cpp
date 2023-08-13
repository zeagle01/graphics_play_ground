
module;

#include <functional>
#include <iostream>
#include "GLFW/glfw3.h"

module main_window:GLFW_wrapper;


namespace quick_shell
{
	class GLFW_wrapper
	{
	public:
		void create_window(int width, int height)
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
		}

		void main_loop(std::function<void(int cursor_x, int cursor_y)> fn)
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

	private:
		GLFWwindow* m_window;
	};
}
