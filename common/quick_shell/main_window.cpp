module;

#include <iostream>
#include "ce_log.h"

module main_window;



namespace quick_shell
{
	void main_window::init(int width,int height)
	{
		m_glfwWrapper.create_window(width, height);
	}

	void main_window::run_event_loop()
	{
		m_glfwWrapper.run_event_loop(
		);
	}

	void main_window::register_frame_update_fn(std::function<void()> frame_update_fn)
	{
		m_glfwWrapper.register_frame_update_fn(frame_update_fn);

	}

	void main_window::register_frame_update_fn(std::function<void(int, int)> frame_update_fn)
	{
		m_glfwWrapper.register_frame_update_fn(frame_update_fn);
	}
}
