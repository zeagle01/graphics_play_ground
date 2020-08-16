
#pragma once

#include "soft_render.h"

namespace soft_render
{

	class Frame_Buffer
	{
	public:
		Frame_Buffer(int width, int height) :m_width(width), m_height(height) {}
		virtual vec4f get_color_f(int x, int y) { return vec4f(); };
		virtual void set_color_f(int x, int y,const vec4f& color) {};

		virtual vec4i get_color_i(int x, int y) { return vec4i(); };
		virtual void set_color_i(int x, int y,const vec4i& color) {};

		int get_width() { return m_width; }
		int get_height() { return m_height; }
	protected:
		int m_width;
		int m_height;

	};

	class Array_Frame_Buffer:public Frame_Buffer
	{
	public:
		Array_Frame_Buffer(int width, int height) :Frame_Buffer(width, height) { m_color.resize(width * height); }

		virtual vec4f get_color_f(int x, int y)override { return m_color[x * m_height + y]; }
		virtual void set_color_f(int x, int y, const vec4f& color) override { m_color[x * m_height + y] = color; }

		virtual vec4i get_color_i(int x, int y) {
			vec4f c_in_f = get_color_f(x, y);
			return  { int(c_in_f[0] * 255),int(c_in_f[1] * 255),int(c_in_f[2] * 255),int(c_in_f[3] * 255) };
		}
		virtual void set_color_i(int x, int y,const vec4i& color) {};

	private:
		std::vector<vec4f> m_color;
	};
}
