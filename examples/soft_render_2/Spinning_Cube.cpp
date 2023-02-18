


#include "Spinning_Cube.h"
#include "Drawing_Buffer.h"
#include "Camara.h"
#include "event.h"

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

		auto press_fn = [this](const MousePress& e)
		{
			m_configs.get_ref<config::camara>().on_press(e.button);
			return true;
		};

		auto release_fn = [this](const MouseRelease& e)
		{
			m_configs.get_ref<config::camara>().on_release(e.button);
			return true;
		};

		auto scroll_fn = [this](const Mouse_Scrolled_Event& e)
		{
			m_configs.get_ref<config::camara>().on_scroll(e.dx,e.dy);
			return true;
		};
		m_screen->add_click_fn(press_fn, release_fn, scroll_fn);

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
				x(0),y(0),z(0),0,
				x(1),y(1),z(1),0,
				x(2),y(2),z(2),0,
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

	mat4 Spinning_Cube::get_projection_matrix()
	{
		auto m_near = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::near>();
		auto m_far = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::far>();
		mat4 ret
		{
			m_near,0,0,0,
			0,m_near,0,0,
			0,0,m_near + m_far,-m_near * m_far,
			0,0,1,0
		};

		return ret;
	}

	float degree_2_radian(float d)
	{
		const float pi = 3.1415926f;
		return d * pi / 180.f;
	}

	mat4 Spinning_Cube::view_port_matrix()
	{

		auto m_near = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::near>();
		auto m_far = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::far>();
		auto m_fov = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::fov>();

		std::array<vec3, 2> screen_space_box{
			{
				{0,0,m_near},
				{float(m_width),float(m_height) ,m_far}
			}

		};

		float tangent = std::tan(degree_2_radian(m_fov));
		float half_height = std::abs(m_near) * tangent;
		float half_width = half_height * m_aspect;

		std::array<vec3, 2> object_space_box{
			{
				{ -half_width  , -half_height  , m_near},
				{  half_width  ,  half_height  , m_far }
			}

		};

		return  get_scale_and_translate(object_space_box, screen_space_box);
	}


	void Spinning_Cube::compute_view_projection_matrix()
	{

		auto camara_matrix = get_camara_matrix();

		auto projection = get_projection_matrix();

		auto view_port = view_port_matrix();

		if (m_configs.get_ref<config::perpective>())
		{
			m_vp = view_port * projection * camara_matrix;
		}
		else
		{
			m_vp = view_port *  camara_matrix;
		}
	}


	void Spinning_Cube::compute_pixel(const vec3& pos,const vec3& color, const mat4& model_matrix)
	{

		mat<4, 1> p{ pos(0), pos(1), pos(2), 1.f };

		p = m_vp * model_matrix * p;

		int xi = p(0) / p(3);
		int yi = p(1) / p(3);
		float depth = p(2) / p(3);

		if (xi >= 0 && xi < m_width && yi >= 0 && yi < m_height)
		{
			m_screen->set_color(xi, yi, depth, color(0), color(1), color(2));
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
				compute_pixel({ x, y, z0 }, { 0.2,0.5,0.8 }, model_matrix);
				compute_pixel({ x, y, z1 }, { 0.2,0.5,0.8 }, model_matrix);
				compute_pixel({ x, z0, y }, { 0.2,0.8,0.5 }, model_matrix);
				compute_pixel({ x, z1, y }, { 0.2,0.8,0.5 }, model_matrix);
				compute_pixel({ z0, x, y }, { 0.5,0.2,0.3 }, model_matrix);
				compute_pixel({ z1, x, y }, { 0.5,0.2,0.3 }, model_matrix);
			}
		}

	}

	void Spinning_Cube::update(int cx, int cy)
	{

		compute_view_projection_matrix();

		m_configs.get_ref<config::camara>().update(cx, cy);

		auto cube_side = m_configs.get_ref<config::cube_side>();
		auto cube_unit = m_configs.get_ref<config::cube_unit>();


		auto model = get_model_matrix({});
		draw_cubic(model, cube_side, cube_unit);
		
		bool draw_axis = false;
		if (draw_axis)
		{
			float litte_cube_side = cube_side / 30;
			float litte_cube_unit = cube_unit / 6;

			const auto& lookat = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::lookat>();
			draw_cubic(get_translate_matrix(lookat), litte_cube_side, litte_cube_unit);

			const auto& cam_location = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::location>();
			const auto& cam_up = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::up>();
			vec3 front_dir = normalize(lookat - cam_location);
			vec3 cam_right = normalize(cross(front_dir, cam_up));

			float axis_length = litte_cube_side * 10;

			auto cam_presentation_location = cam_location + 5 * axis_length * front_dir;
			draw_cubic(get_translate_matrix(cam_presentation_location), litte_cube_side, litte_cube_unit);
			draw_cubic(get_translate_matrix(cam_presentation_location + 1 * axis_length * cam_right), litte_cube_side, litte_cube_unit);
			draw_cubic(get_translate_matrix(cam_presentation_location + 2 * axis_length * cam_up), litte_cube_side, litte_cube_unit);
			draw_cubic(get_translate_matrix(cam_presentation_location + 3 * axis_length * front_dir), litte_cube_side, litte_cube_unit);

		}

		const auto& angle_rate = get_config<config::angle_rate>();
		auto& init_angle = get_config<config::init_angle>();
		init_angle = init_angle + angle_rate;
	}
}
