
#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <array>
#include "mat.h"
#include "event.h"

namespace soft_render
{
	class Rasterizer;
	class Drawing_Buffer
	{
	public:

		void init(int width, int height);

		void main_loop(std::function<void(int, int)> fn);

		void set_color(int wi, int hi, float depth, float r, float g, float b);
		

		void draw_line(const std::array<vec3, 2>& x, std::function<vec3(int, int, float)> fragment_shader);
		void draw_triangle(const std::array<vec3, 3>& x, std::function<vec3(vec3)> fragment_shader);

		void clear();
		void draw();

	private:
		std::vector<uint32_t> buffer;
		std::vector<float > depth_buffer;
		int m_width;
		int m_height;
		std::shared_ptr<Rasterizer> m_rasterizer;
	};

}