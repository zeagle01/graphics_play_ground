


#include "Spinning_Cube.h"
#include "Drawing_Buffer.h"
#include "Camara.h"

#include "member_extractor.h"


namespace soft_render
{


	void Spinning_Cube::set_spinning_cube_default_value()
	{
		type_map::fill_types<config>(m_configs);
	}

	void Spinning_Cube::init(int w, int h, Drawing_Buffer* sc)
	{
		m_width = w;
		m_height = h;

		m_aspect = float(m_width) / float(m_height);

		m_screen = sc;

		auto press_fn = [this]()
		{
			m_configs.get_ref<config::camara>().on_press();
		};

		auto release_fn = [this]()
		{
			m_configs.get_ref<config::camara>().on_release();
		};
		m_screen->add_click_fn(press_fn, release_fn);

	}

	mat4 Spinning_Cube::get_translate_matrix(const vec3& translate)
	{

		mat4 ret
		{
				1,0,0,translate(0),
				0,1,0,translate(1),
				0,0,1,translate(2),
				0,0,0,1
		};
		return  ret;
	}

	mat4 Spinning_Cube::get_model_matrix(const vec3& translate)
	{
		auto init_angle = m_configs.get_ref<config::init_angle>();
		float m_angle_x = init_angle(0);
		float m_angle_y = init_angle(1);
		float m_angle_z = init_angle(2);

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

		return rotz * roty * rotx * get_translate_matrix(translate);
	}

	mat4 Spinning_Cube::get_camara_matrix()
	{
		const auto& m_lookat = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::lookat>();
		const auto& m_camara_location = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::location>();
		const auto& m_camara_up = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::up>();

		vec3 z = normalize(m_camara_location - m_lookat);
		vec3 x = normalize(cross(m_camara_up, z));
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

		auto dx_from = box_from[1] - box_from[0];


		mat4 translate_to_dst = get_identity<4>();

		translate_to_dst(0, 3) = box_dst[0](0);
		translate_to_dst(1, 3) = box_dst[0](1);
		translate_to_dst(2, 3) = box_dst[0](2);

		mat4 scale_to_dst = get_identity<4>();
		auto dx_dst = box_dst[1] - box_dst[0];

		scale_to_dst(0, 0) = dx_dst(0) / dx_from(0);
		scale_to_dst(1, 1) = dx_dst(1) / dx_from(1);
		scale_to_dst(2, 2) = dx_dst(2) / dx_from(2);

		return  translate_to_dst * scale_to_dst * translate_back_to_origin;
	}

	mat4 Spinning_Cube::view_port_matrix()
	{
		std::array<vec3, 2> screen_space_box{
			{
				{0,0,0},
				{float(m_width),float(m_height) ,1.f}
			}

		};


		std::array<vec3, 2> object_space_box{
			{
				{ -0.5f * m_world_height  , -0.5f * m_world_height ,0},
				{  0.5f * m_world_height  ,  0.5f * m_world_height  , -m_world_height }
			}

		};

		return  get_scale_and_translate(object_space_box, screen_space_box);
	}




	void Spinning_Cube::compute_pixel(float x, float y, float z, const mat4& model_matrix)
	{

		mat<4, 1> p{ {x,y,z,1.f} };
		auto camara_matrix = get_camara_matrix();

		auto view_port = view_port_matrix();
		p = view_port * camara_matrix * model_matrix * p;

		int xi = p(0);
		int yi = p(1);

		if (xi >= 0 && xi < m_width && yi >= 0 && yi < m_height)
		{
			m_screen->set_color(xi, yi, 0.2, 0.3, 0.5);
		}

	}

	void Spinning_Cube::draw_cubic(const mat4& model_matrix, float cube_side, float cube_unit)
	{
		for (float xi = 0; xi <  cube_side; xi += cube_unit)
		{
			for (float yi = 0; yi < cube_side; yi += cube_unit)
			{
				float x = xi ;
				float y = yi ;
				float z0 = 0 ;
				float z1 = cube_side ;
				compute_pixel(x, y, z0, model_matrix);
				compute_pixel(x, y, z1, model_matrix);
				compute_pixel(x, z0, y, model_matrix);
				compute_pixel(x, z1, y, model_matrix);
				compute_pixel(z0, x, y, model_matrix);
				compute_pixel(z1, x, y, model_matrix);
			}
		}

	}

	void Spinning_Cube::update(int cx, int cy)
	{

		m_configs.get_ref<config::camara>().update(cx, cy);

		auto cube_side = m_configs.get_ref<config::cube_side>();
		auto cube_unit = m_configs.get_ref<config::cube_unit>();


		auto model = get_model_matrix({});
		draw_cubic(model, cube_side, cube_unit);
		
		{
			float litte_cube_side = cube_side / 30;
			float litte_cube_unit = cube_unit / 6;

			const auto& lookat = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::lookat>();
			draw_cubic(get_translate_matrix(lookat), litte_cube_side, litte_cube_unit);

			const auto& cam_location = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::location>();
			draw_cubic(get_translate_matrix(cam_location), litte_cube_side, litte_cube_unit);


			float axis_length = litte_cube_side * 10;
			const auto& cam_up = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::up>();
			draw_cubic(get_translate_matrix(cam_location + 2 * axis_length * cam_up), litte_cube_side, litte_cube_unit);

			vec3 front_dir = normalize(lookat - cam_location);
			draw_cubic(get_translate_matrix(cam_location + 3 * axis_length * front_dir), litte_cube_side, litte_cube_unit);

			vec3 cam_right = normalize(cross(front_dir, cam_up));
			draw_cubic(get_translate_matrix(cam_location + 1 * axis_length * cam_right), litte_cube_side, litte_cube_unit);

		}

		const auto& angle_rate = get_config<config::angle_rate>();
		auto& init_angle = get_config<config::init_angle>();
		init_angle = init_angle + angle_rate;
	}
}
