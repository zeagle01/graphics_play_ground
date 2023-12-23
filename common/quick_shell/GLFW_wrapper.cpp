
module;

#include <functional>
#include <iostream>
#include "GLFW/glfw3.h"
#include <ranges>
#include <algorithm>

#include "ce_log.h";

module quick_shell:GLFW_wrapper;

import :ui_event;

import clumsy_lib;

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

			glfwSetWindowUserPointer(m_window, &m_dispatchor);

			register_GLFW_event_callback();
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

		template<typename concrete_event>
		void register_event_fn(std::function<bool(const concrete_event&)> event_fn)
		{
			m_dispatchor.connect<concrete_event>(event_fn);
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

		GLFWwindow* get_window_ptr() { return m_window; }

	private:
		void register_GLFW_event_callback()
		{

			glfwSetWindowSizeCallback(m_window,
				[](GLFWwindow* window, int w, int h)
				{

					auto& dispatchor = *(clumsy_lib::dispatchor <ui_event, bool>*)glfwGetWindowUserPointer(window);
					window_resize_event e;
					e.width = w;
					e.height = h;
					dispatchor(e);
				}
			);

			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
				{
					auto& dispatchor = *(clumsy_lib::dispatchor <ui_event, bool>*)glfwGetWindowUserPointer(window);
					window_close_event e;
					dispatchor(e);
				}
			);

			glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scan_code, int action, int mods)
				{
					auto& dispatchor = *(clumsy_lib::dispatchor <ui_event, bool>*)glfwGetWindowUserPointer(window);

					switch (action)
					{
						case GLFW_PRESS:
						{
							key_pressed e;
							e.key = static_cast<key_code>(key);
							e.repeat_count = 0;
							dispatchor(e);
							break;
						}
						case GLFW_REPEAT:
						{
							key_pressed e;
							e.key = static_cast<key_code>(key);
							e.repeat_count = 1;
							dispatchor(e);
							break;
						}
						case GLFW_RELEASE:
						{
							key_released e;
							e.key = static_cast<key_code>(key);
							dispatchor(e);
							break;
						}
					}
				}
			);

			glfwSetMouseButtonCallback(m_window,
				[](GLFWwindow* window, int button, int action, int mods)
				{
					auto& dispatchor = *(clumsy_lib::dispatchor <ui_event, bool>*)glfwGetWindowUserPointer(window);

					double cposx, cposy;
                    glfwGetCursorPos(window, &cposx, &cposy);

					switch (action)
					{
					case GLFW_PRESS:
					{
						mouse_pressed e;
						e.button = static_cast<mouse_button>(button);
						e.x = cposx;
						e.y = cposy;
						dispatchor(e);
						break;
					}
					case GLFW_RELEASE:
					{
						mouse_released e;
						e.button = static_cast<mouse_button>(button);
						e.x = cposx;
						e.y = cposy;
						dispatchor(e);
						break;
					}
					}

				}
			);

			glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
				{
					auto& dispatchor = *(clumsy_lib::dispatchor <ui_event, bool>*)glfwGetWindowUserPointer(window);

					mouse_scrolled_event e; 
					e.dx = xoffset;
					e.dy = yoffset;
					dispatchor(e);
				}
			);

			glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
				{

					auto& dispatchor = *(clumsy_lib::dispatchor <ui_event, bool>*)glfwGetWindowUserPointer(window);

					mouse_moved e; (x, y);
					e.x = x;
					e.y = y;
					dispatchor(e);

				}
			);

		}


		GLFWwindow* m_window;
		std::vector<std::function<void(int, int)>> m_frame_update_fns;

		clumsy_lib::dispatchor<ui_event, bool> m_dispatchor;

	};
}
