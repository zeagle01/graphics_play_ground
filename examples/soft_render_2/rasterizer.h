
#pragma once

#include "mat.h"
#include <functional>


namespace soft_render
{


	class Rasterizer
	{
	public:
		void loop_line(vec2i v0, float depth0, vec2i v1, float depth1, std::function<void(int, int, float)> fn)
		{
			bool steep = false;
			if (std::abs(v0(0) - v1(0)) < std::abs(v0(1) - v1(1)))
			{
				std::swap(v0(0), v0(1));
				std::swap(v1(0), v1(1));
				steep = true;
			}

			if (v0(0) > v1(0))
			{
				std::swap(v0, v1);
			}

			for (int x = v0(0); x < v1(0); x++)
			{
				float t = (float)(v1(1) - v0(1)) / (float)(v1(0) - v0(0));
				int y = v0(1) + t * (x - v0(0));
				float r = (x - v0(0)) / (v1(0) - v0(0));
				float depth = depth0 + r * (depth1 - depth0);
				if (steep)
				{
					fn(y, x,depth);
				}
				else
				{
					fn(x, y,depth);
				}
			}
		}

		void triangle_fragment_loop(vec2i v0, vec2i v1,vec2i v2, std::function<void(int, int)> fn)
		{
			if (v0(1) > v1(1)) std::swap(v0, v1);
			if (v0(1) > v2(1)) std::swap(v0, v2);
			if (v1(1) > v2(1)) std::swap(v1, v2);
			//int total_height
			for (int y = v0(1); y < v1(1); y++)
			{
				float t1 = float(y - v0(1)) / float(v1(1) - v0(1));
				float t2 = float(y - v0(1)) / float(v2(1) - v0(1));
				int xa = v0(0) + (v1(0) - v0(0)) * t1;
				int xb = v0(0) + (v2(0) - v0(0)) * t2;

				int xmin = xa < xb ? xa : xb;
				int xmax = xa > xb ? xa : xb;
				for (int x = xmin; x < xmax; x++)
				{
					fn(x,y);
				}
			}

			for (int y = v1(1); y < v2(1); y++)
			{
				float t1 = float(y - v1(1)) / float(v2(1) - v1(1));
				float t2 = float(y - v0(1)) / float(v2(1) - v0(1));
				int xa = v1(0) + (v2(0) - v1(0)) * t1;
				int xb = v0(0) + (v2(0) - v0(0)) * t2;

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