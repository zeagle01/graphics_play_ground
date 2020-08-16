
#pragma once


#include "frame_buffer.h"


namespace soft_render
{


	class Rasterizer
	{
	public:
		virtual void loop_line(vec2i v0, vec2i v1, std::function<void(int, int)> fn)
		{
			bool steep = false;
			if (std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y))
			{
				std::swap(v0.x, v0.y);
				std::swap(v1.x, v1.y);
				steep = true;
			}

			if (v0.x > v1.x)
			{
				std::swap(v0, v1);
			}

			for (int x = v0.x; x < v1.x; x++)
			{
				float t = (float)(v1.y - v0.y) / (float)(v1.x - v0.x);
				int y = v0.y + t * (x - v0.x);
				if (steep)
				{
					fn(y, x);
				}
				else
				{
					fn(x, y);
				}
			}
		}

		virtual void triangle_fragment_loop(vec2i v0, vec2i v1,vec2i v2, std::function<void(int, int)> fn)
		{
			if (v0.y > v1.y) std::swap(v0, v1);
			if (v0.y > v2.y) std::swap(v0, v2);
			if (v1.y > v2.y) std::swap(v1, v2);
			//int total_height
			for (int y = v0.y; y < v1.y; y++)
			{
				float t1 = float(y - v0.y) / float(v1.y - v0.y);
				float t2 = float(y - v0.y) / float(v2.y - v0.y);
				int xa = v0.x + (v1.x - v0.x) * t1;
				int xb = v0.x + (v2.x - v0.x) * t2;

				int xmin = xa < xb ? xa : xb;
				int xmax = xa > xb ? xa : xb;
				for (int x = xmin; x < xmax; x++)
				{
					fn(x,y);
				}
			}

			for (int y = v1.y; y < v2.y; y++)
			{
				float t1 = float(y - v1.y) / float(v2.y - v1.y);
				float t2 = float(y - v0.y) / float(v2.y - v0.y);
				int xa = v1.x + (v2.x - v1.x) * t1;
				int xb = v0.x + (v2.x - v0.x) * t2;

				int xmin = xa < xb ? xa : xb;
				int xmax = xa > xb ? xa : xb;
				for (int x = xmin; x < xmax; x++)
				{
					fn(x,y);
				}
			}
		}
	};

}