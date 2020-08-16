
#pragma once


#include "frame_buffer.h"


namespace soft_render
{


	class Rasterizer
	{
	public:
		Rasterizer(Frame_Buffer& fb) :m_frame_buffer(fb) {};

		void set_frame_buffer(Frame_Buffer& fb) { m_frame_buffer = fb; }

		virtual void line(int x0, int y0, int x1, int y1, vec4f color)
		{

			bool steep = false;
			if (std::abs(x0 - x1) < std::abs(y0 - y1))
			{
				std::swap(x0, y0);
				std::swap(x1, y1);
				steep = true;
			}

			if (x0 > x1)
			{
				std::swap(x0, x1);
				std::swap(y0, y1);
			}

			for (int x = x0; x < x1; x++)
			{
				float t = (float)(y1 - y0) / (float)(x1 - x0);
				int y = y0 + t * (x - x0);
				if (steep)
				{
					m_frame_buffer.set_color_f(y, x, color);
				}
				else
				{
					m_frame_buffer.set_color_f(x, y, color);
				}
			}
		}

	protected:
		Frame_Buffer& m_frame_buffer;
	};

}