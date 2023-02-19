
#include "Drawing_Buffer.h"

#include "OpenGL_Wrapper.h"
#include "rasterizer.h"

#include <functional>
#include <limits>

namespace soft_render
{

	OpenGL_Wrapper gl;

	void Drawing_Buffer::init(int width, int height)
	{
		m_width = width;
		m_height = height;
		gl.init(width, height);
		buffer.resize(width * height);
		depth_buffer.resize(width * height);
		m_rasterizer = std::make_shared<Rasterizer>();

		clear();
	}


	void Drawing_Buffer::clear()
	{
		for (int i = 0; i < buffer.size(); i++)
		{
			buffer[i] = 0xffffffff;
			depth_buffer[i] = std::numeric_limits<float>::lowest();
		}
	}

	void Drawing_Buffer::draw()
	{
		gl.draw(buffer.data());
	}

	void Drawing_Buffer::set_color(int wi, int hi, float depth, float r, float g, float b)
	{
		bool within_screen = hi < m_height&& hi >= 0 && wi >= 0 && wi < m_width;
		if (!within_screen)
		{
			return;
		}

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

	void Drawing_Buffer::draw_line(const std::array<vec3, 2>& x, const vec3& color)
	{
		std::array<vec2i, 2> xi = { x[0](0),x[0](1) ,  x[1](0),x[1](1) };

		m_rasterizer->loop_line(xi,
			[this,x,color](int i, int j,float ratio) 
			{
				float depth = x[0](2) + ratio * (x[1](2) - x[0](2));
				set_color(i, j, depth, color(0), color(1), color(2));
			}
		);

	}

	void Drawing_Buffer::draw_triangle(const std::array<vec3, 3>& x, const vec3& color)
	{
		std::array<vec2i, 3> xi = { x[0](0),x[0](1) ,  x[1](0),x[1](1),  x[2](0),x[2](1)};
		m_rasterizer->triangle_fragment_loop(xi,
			[this, x, color](int i, int j, const vec3& w)
			{
				float depth = w(0) * x[0](2) + w(1) * x[1](2) + w(2) * x[2](2);
				set_color(i, j, depth, color(0), color(1), color(2));
			});

	}
}
