
#pragma once

#include <vector>
#include <functional>
#include <memory>
#include "event.h"

namespace soft_render
{

	class Drawing_Buffer
	{
	public:

		void init(int width, int height);

		void main_loop(std::function<void(int, int)> fn);

		void set_color(int wi, int hi, float depth, float r, float g, float b);

		void clear();
		void draw();

	private:
		std::vector<uint32_t> buffer;
		std::vector<float > depth_buffer;
		int m_width;
		int m_height;

	};

}