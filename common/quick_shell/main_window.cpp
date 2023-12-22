module;

#include <iostream>
#include "ce_log.h"

module quick_shell;



namespace quick_shell
{
	//main window
	void main_window::init(int width,int height)
	{
		m_glfwWrapper.create_window(width, height);
		m_ui_panel.init(m_glfwWrapper.get_window_ptr());
		m_renderer.load_glad(GLFW_wrapper::get_proc_address());

		m_ui_fn = [this]() { m_ui_panel.upate_ui_data(); };
	}

	void main_window::run_event_loop()
	{
		m_glfwWrapper.register_frame_update_fn(m_ui_fn);
		m_glfwWrapper.run_event_loop();
	}

	void main_window::register_frame_update_fn(std::function<void()> frame_update_fn)
	{
		m_glfwWrapper.register_frame_update_fn(frame_update_fn);

	}

	void main_window::register_frame_update_fn(std::function<void(int, int)> frame_update_fn)
	{
		m_glfwWrapper.register_frame_update_fn(frame_update_fn);
	}


	//renderer
	void renderer::draw_triangles(int* indices, float* pos, int tNum, int vNum)
	{
		m_render_imp.draw_triangles(indices, pos, tNum, vNum);
	}

	void renderer::enable_fill(bool isLine)
	{
		m_render_imp.enable_fill(isLine);
	}

	void renderer::enable_wireframe(bool isLine)
	{
		m_render_imp.enable_wireframe(isLine);
	}

	void renderer::draw_points(float* pos, int vNum)
	{
		m_render_imp.draw_points(pos, vNum);
	}
}
