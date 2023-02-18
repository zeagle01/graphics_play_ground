
#pragma once

#include "dispatcher.h"
#include "event.h"
#include <functional>


struct GLFWwindow;

namespace soft_render
{
	class GLFW_Wrapper
	{
	public:
		void create_window(int width, int height);
		void main_loop(std::function<void(int,int)> fn);


		template<typename E>
		void add_event_handler(std::function<bool(const E&)> handler)
		{
			m_dispatcher.add<E>(handler);
		}

		void setup_input();

		GLFWwindow* get_window() { return m_window; }

	private:
		GLFWwindow* m_window;

		dispatcher<Event, bool> m_dispatcher;
	};
}