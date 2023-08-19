
module;

#include <functional>
#include <iostream>
#include "GLFW/glfw3.h"
#include <ranges>
#include <algorithm>

#include "ce_log.h";

module main_window:GLFW_wrapper;



namespace quick_shell
{
	using proc_address_t = decltype(glfwGetProcAddress);

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

		void register_frame_update_fn(std::function<void()> frame_update_fn)
		{
			m_frame_update_fns.push_back(
				[=](int, int) {frame_update_fn(); }
			);
		}

		void register_frame_update_fn(std::function<void(int,int)> frame_update_fn)
		{
			m_frame_update_fns.push_back(frame_update_fn);
		}

		void run_event_loop()
		{
			while (!glfwWindowShouldClose(m_window))
			{
				glfwPollEvents();

				double cx;
				double cy;

				glfwGetCursorPos(m_window, &cx, &cy);

				std::ranges::for_each(m_frame_update_fns, [&](auto& fn) { fn(int(cx), int(cy)); });

				glfwMakeContextCurrent(m_window); //in case for docking gui

				glfwSwapBuffers(m_window);
			}
		}


		static proc_address_t* get_proc_address() 
		{
			return &glfwGetProcAddress;
		}

	private:
		GLFWwindow* m_window;
		std::vector<std::function<void(int, int)>> m_frame_update_fns;
	};
}
