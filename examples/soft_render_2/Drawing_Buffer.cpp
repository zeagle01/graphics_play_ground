
#include "Drawing_Buffer.h"

#include "glad/glad.h"

#include "GLFW_Wrapper.h"

#include "OpenGL_Wrapper.h"

#include <functional>
#include <limits>

namespace soft_render
{


	OpenGL_Wrapper gl;

	void Drawing_Buffer::init(int width, int height)
	{
		glfw_win = std::make_shared<GLFW_Wrapper>();
		m_width = width;
		m_height = height;
		m_window = glfw_win->create_window(width, height);
		gl.init(width, height);
		buffer.resize(width * height);
		depth_buffer.resize(width * height);

		clear();
	}

	void Drawing_Buffer::add_click_fn(std::function<void()> pressed_fn, std::function<void()> release_fn)
	{
		glfw_win->add_click_fn(pressed_fn, release_fn);
	}


	void Drawing_Buffer::main_loop(std::function<void(int ,int)> fn)
	{
		glfw_win->main_loop(
			[this, fn]()
			{

				gl.clear();
				gl.draw(buffer.data());

				double cx;
				double cy;

				glfwGetCursorPos(m_window, &cx, &cy);

				fn(int(cx), int(cy));
			});
	}

	void Drawing_Buffer::clear()
	{
		for (int i = 0; i < buffer.size(); i++)
		{
			buffer[i] = 0xffffffff;
			depth_buffer[i] = std::numeric_limits<float>::lowest();
		}
	}

	void Drawing_Buffer::set_color(int wi, int hi, float depth, float r, float g, float b)
	{
		if (depth > depth_buffer[hi * m_width + wi])
		{
			int ri = r * 255;
			int gi = g * 255;
			int bi = b * 255;

			buffer[hi * m_width + wi] =
				(255 << 24) + (int(bi) << 16) + (int(gi) << 8) + int(ri);

			depth_buffer[hi * m_width + wi] = depth;
		}
	}
}
