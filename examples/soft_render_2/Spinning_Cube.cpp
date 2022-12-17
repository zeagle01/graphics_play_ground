


#include "Spinning_Cube.h"
#include "Drawing_Buffer.h"


namespace soft_render
{

	Spinning_Cube::Spinning_Cube(int w, int h, Drawing_Buffer* sc)
		:m_width(w), m_height(h), screen(sc)
	{
		m_datas.add_type<config::angle_rate>();
	}

	mat4 Spinning_Cube::get_rotation_matrix()
	{
		float cx = std::cos(m_angle_x);
		float sx = std::sin(m_angle_x);
		mat4 rotx
		{
			{
				1,0,0,0,
				0,cx,-sx,0,
				0,sx,cx,0,
				0,0,0,1
			}
		};

		float cy = std::cos(m_angle_y);
		float sy = std::sin(m_angle_y);
		mat4 roty
		{
			{
				cy,0,-sy,0,
				0,1,0,0,
				sy,0,cy,0,
				0,0,0,1
			}
		};

		float cz = std::cos(m_angle_z);
		float sz = std::sin(m_angle_z);
		mat4 rotz
		{
			{
				cz,-sz,0,0,
				sz,cz,0,0,
				0,0,1,0,
				0,0,0,1
			}
		};

		return rotz * roty * rotx;
	}

	mat4 Spinning_Cube::get_camara_matrix()
	{

		vec3 z = normalize(m_camara_location - m_lookat);
		vec3 x = normalize(cross(m_up, z));
		vec3 y = normalize(cross(z, x));

		mat4 frame
		{
			{
				x.data[0],y.data[0],z.data[0],0,
				x.data[1],y.data[1],z.data[1],0,
				x.data[2],y.data[2],z.data[2],0,
				0,0,0,1,
			}
		};


		mat4 trans{
			{
				1,0,0,-m_camara_location.data[0],
				0,1,0,-m_camara_location.data[1],
				0,0,1,-m_camara_location.data[2],
				0,0,0,1.f
			}
		};


		return frame * trans;
	}

	mat4 Spinning_Cube::get_scale_and_translate(const std::array<vec3, 2>& box_from, const std::array<vec3, 2>& box_dst)
	{
		mat4 translate_back_to_origin = get_identity<4>();

		translate_back_to_origin(0, 3) = -box_from[0](0);
		translate_back_to_origin(1, 3) = -box_from[0](1);
		translate_back_to_origin(2, 3) = -box_from[0](2);

		mat4 scale_back_to_cubic = get_identity<4>();
		auto dx_from = box_from[1] - box_from[0];

		scale_back_to_cubic(0, 0) = 1.f / dx_from(0);
		scale_back_to_cubic(1, 1) = 1.f / dx_from(1);
		scale_back_to_cubic(2, 2) = 1.f / dx_from(2);

		mat4 translate_to_dst = get_identity<4>();

		translate_to_dst(0, 3) = box_dst[0](0);
		translate_to_dst(1, 3) = box_dst[0](1);
		translate_to_dst(2, 3) = box_dst[0](2);

		mat4 scale_to_dst = get_identity<4>();
		auto dx_dst = box_dst[1] - box_dst[0];

		scale_to_dst(0, 0) = dx_dst(0);
		scale_to_dst(1, 1) = dx_dst(1);
		scale_to_dst(2, 2) = dx_dst(2);

		return  translate_to_dst * scale_to_dst * scale_back_to_cubic * translate_back_to_origin;
	}

	mat4 Spinning_Cube::to_pixel_space()
	{
		std::array<vec3, 2> screen_space_box{
			{
				{0,0,0},
				{float(m_width),float(m_height) ,1.f}
			}

		};

		std::array<vec3, 2> object_space_box{
			{
				{m_lookat(0) - m_aspect * m_world_height / 2  ,m_lookat(1) - m_world_height / 2 ,-1},
				{m_lookat(0) + m_aspect * m_world_height / 2 , m_lookat(1) + m_world_height / 2 ,1 }
			}

		};

		return  get_scale_and_translate(object_space_box, screen_space_box);
	}




	void Spinning_Cube::compute_pixel(float x, float y, float z)
	{
		mat<4, 1> p{ {x,y,z,1.f} };
		auto rotx = get_rotation_matrix();
		//auto camara_matrix = get_camara_matrix();
		//p = camara_matrix * rotx * p;

		auto m = to_pixel_space();
		p = m * rotx * p;
		int xi = p(0);
		int yi = p(1);
		if (xi >= 0 && xi < m_width && yi >= 0 && yi < m_height)
		{
			screen->set_color(xi, yi, 0.2, 0.3, 0.5);
		}

	}

	void Spinning_Cube::update()
	{

		for (float xi = 0.f; xi < cube_side; xi += cube_unit)
		{
			for (float yi = 0.f; yi < cube_side; yi += cube_unit)
			{
				compute_pixel(xi, yi, 0.f);
				compute_pixel(xi, yi, cube_side);
				compute_pixel(xi, 0, yi);
				compute_pixel(xi, cube_side, yi);
				compute_pixel(0, xi, yi);
				compute_pixel(cube_side, xi, yi);
			}
		}

		const auto& angle_rate = get_config<config::angle_rate>();
		m_angle_x += angle_rate(0);
		m_angle_y += angle_rate(1);
		m_angle_z += angle_rate(2);
	}
}
