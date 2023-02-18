
#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "event.h"

struct GLFWwindow;

namespace soft_render
{

	class GLFW_Wrapper;

	class Drawing_Buffer
	{
	public:

		void init(int width, int height);

		void main_loop(std::function<void(int, int)> fn);

		void set_color(int wi, int hi, float depth, float r, float g, float b);

		void add_click_fn(std::function<bool(const MousePress&)>  pressed_fn, std::function<bool(const MouseRelease&)> release_fn);

		void clear();

		GLFWwindow* get_window() { return m_window; }

	private:
		std::vector<uint32_t> buffer;
		std::vector<float > depth_buffer;
		int m_width;
		int m_height;

		GLFWwindow* m_window;
		std::shared_ptr<GLFW_Wrapper> glfw_win;


	};

}