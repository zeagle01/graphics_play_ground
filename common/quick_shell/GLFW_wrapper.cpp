
module;

#include <functional>
#include <iostream>
#include "GLFW/glfw3.h"

#include "ce_log.h";

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
				CE_LOG_INFO("glfw init succeed! ");
			}
			else
			{
				CE_LOG_INFO("glfw init error! ");
			}

			GLFWwindow* window;
			window = glfwCreateWindow(width, height, "gl window", nullptr, nullptr);

			if (window)
			{
				CE_LOG_INFO(" window {} is created", (void*)window);
			}
			else
			{
				CE_LOG_ERROR(" window can't be created");
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
