

module;

#include <vector>
#include <memory>

module app:plane_maker;

import :components;
import :matrix_def;


class plane_maker
{
public:
	void create_plane(position_buffer& positions, triangle_index_buffer& triangles, float length_x, float length_y, int resolution_x, int resolution_y)
	{

		int vNum = (resolution_x + 1) * (resolution_y + 1);
		int tNum = resolution_x * resolution_y * 2;

		positions.num = vNum;
		triangles.num = tNum;

		positions.data = std::shared_ptr<vec3f[]>(new vec3f[vNum]);
		triangles.data = std::shared_ptr<vec3i[]>(new vec3i[tNum]);

		float dx = length_x / resolution_x;
		float dy = length_y / resolution_y;

		int vi = 0;
		for (int i = 0; i < resolution_x + 1; i++)
		{
			for (int j = 0; j < resolution_y + 1; j++)
			{
				auto ptr = positions.data.get();
				ptr[vi] = vec3f({ dx * i, dy * j, 0 });
				vi++;
			}
		}

		int ti = 0;
		for (int i = 0; i < resolution_x; i++)
		{
			for (int j = 0; j < resolution_y; j++)
			{
				int v0 = i * (resolution_y + 1) + j;
				int v1 = i * (resolution_y + 1) + j + 1;
				int v2 = (i + 1) * (resolution_y + 1) + j + 1;
				int v3 = (i + 1) * (resolution_y + 1) + j ;

				auto ptr = triangles.data.get();
				ptr[ti] = vec3i({ v0,v1,v2 });
				ti++;
				ptr[ti] = vec3i({ v0,v2,v3 });
				ti++;
			}
		}

	}
};