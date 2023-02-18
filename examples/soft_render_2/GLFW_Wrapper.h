
#pragma once

#include "dispatcher.h"
#include "GLFW/glfw3.h"
#include "event.h"
#include <functional>


struct GLFWwindow;

namespace soft_render
{





	class GLFW_Wrapper
	{
	public:
		GLFWwindow* create_window(int width, int height);
		void main_loop(std::function<void()>);


		template<typename E>
		void add_event_handler(std::function<bool(const E&)> handler)
		{
			m_dispatcher.add<E>(handler);
		}

		void setup_input();

	private:
		GLFWwindow* m_window;


		dispatcher <Event, bool> m_dispatcher;
	};
}