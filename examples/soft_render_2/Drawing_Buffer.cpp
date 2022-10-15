
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
	window = glfw_win.create_window(width, height);
	gl.init(width, height);
	imgui_wrapper.init(window);
	buffer.resize(width * height);

	for (int i = 0; i < buffer.size(); i++)
	{
		buffer[i] = 0xffffffff - i;
	}
}

void Drawing_Buffer::main_loop(std::function<void(uint32_t*)> fn)
{
	glfw_win.main_loop(
		[this, fn]()
		{
			gl.clear();

			fn(buffer.data());

			gl.draw(buffer.data());

			imgui_wrapper.update();

		});
}
