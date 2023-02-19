
#pragma once

#include "mat.h"
#include <functional>
#include <array>


namespace soft_render
{


	class Rasterizer
	{
	public:
		void loop_line(const std::array<vec2i, 2>& p, std::function<void(int, int, float)> fn)
		{
			vec2i v0{ p[0](0),p[0](1) };
			vec2i v1{ p[1](0),p[1](1) };

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
				if (steep)
				{
					fn(y, x, r);
				}
				else
				{
					fn(x, y, r);
				}
			}
		}

		void triangle_fragment_loop(const std::array<vec2i,3>& p, std::function<void(int, int, const vec3& )> fn)
		{
			vec2i v0{ p[0](0),p[0](1) };
			vec2i v1{ p[1](0),p[1](1) };
			vec2i v2{ p[2](0),p[2](1) };

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
					auto w = compute_baricentric(p, { x,y });
					fn(x,y,w);
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
					auto w = compute_baricentric(p, { x,y });
					fn(x, y, w);
				}
			}
		}
	private:

		vec3 compute_baricentric(const std::array<vec2i,3>& triangle_i, const vec2i& x_i)
		{
			std::array<vec2, 3> triangle = { triangle_i[0](0),triangle_i[0](1),triangle_i[1](0),triangle_i[1](1),triangle_i[2](0),triangle_i[2](1) };
			vec2 x{ x_i(0),x_i(1) };
			vec2 dv0 = triangle[0] - triangle[2];
			vec2 dv1 = triangle[1] - triangle[2];
			vec2 b = x - triangle[2];

			mat2 A{
				dv0(0),dv1(0),
				dv0(1),dv1(1)
			};
			float det = A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1);
			mat2 invA
			{
				A(1,1),-A(0,1),
				-A(1,0),A(0,0)
			};

			auto w = invA * x / det;
			return vec3{ w(0),w(1),1 - w(0) - w(1) };
		}
	};

}