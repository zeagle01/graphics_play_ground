
#include "Drawing_Buffer.h"

#include "glad/glad.h"

#include "GLFW_Wrapper.h"

#include "Imgui_Wrapper.h"

#include "OpenGL_Wrapper.h"


GLFW_Wrapper glfw_win;
OpenGL_Wrapper gl;
Imgui_Wrapper imgui_wrapper;
GLFWwindow* window;

void Drawing_Buffer::init(int width, int height)
{
	m_width = width;
	m_height = height;
	window = glfw_win.create_window(width, height);
	gl.init(width, height);
	imgui_wrapper.init(window);
	buffer.resize(width * height);

	clear();
}

void Drawing_Buffer::main_loop(std::function<void()> fn)
{
	glfw_win.main_loop(
		[this, fn]()
		{
			gl.clear();

			fn();

			gl.draw(buffer.data());

			imgui_wrapper.update();

		});
}

void Drawing_Buffer::clear()
{
	for (int i = 0; i < buffer.size(); i++)
	{
		buffer[i] = 0xffffffff ;
	}
}

void Drawing_Buffer::set_color(int wi, int hi, float r, float g, float b)
{
	int ri = r * 255;
	int gi = g * 255;
	int bi = b * 255;

	buffer[hi * m_width + wi] =
		(255 << 24) + (int(bi) << 16) + (int(gi) << 8) + int(ri);
}
