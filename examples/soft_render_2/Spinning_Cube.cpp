


#include "Spinning_Cube.h"
#include "Drawing_Buffer.h"
#include "Camara.h"
#include "mat.h"
#include "shader.h"
#include "mesh_loader.h"
#include "graphic_math.h"

#include "member_extractor.h"

#include <functional>
#include <algorithm>


namespace soft_render
{

	void Spinning_Cube::init(int w, int h, Drawing_Buffer* sc)
	{
		type_map::fill_types<config>(m_configs);

		m_width = w;
		m_height = h;

		m_aspect = float(m_width) / float(m_height);

		m_screen = sc;
		m_shader = std::make_shared<Shader>();
		m_shader->init(sc);

		m_mesh_loader = std::make_shared<Mesh_Loader>();

		if (m_configs.get_ref<config::draw_mesh>())
		{
			m_mesh_loader->load(m_configs.get_ref<config::mesh_file>());
		}
	}


	bool Spinning_Cube::on_mouse_pressed(const Mouse_Pressed& e)
	{
		if (e.button == CE_MOUSE_BUTTON_RIGHT)
		{
			m_configs.get_ref<config::camara>().drag_begin();
		}
		return true;
	}

	bool Spinning_Cube::on_mouse_released(const Mouse_Released& e)
	{
		if (e.button == CE_MOUSE_BUTTON_RIGHT)
		{
			m_configs.get_ref<config::camara>().drag_end();
		}
		return true;
	}

	bool Spinning_Cube::on_mouse_scroll(const Mouse_Scrolled_Event& e)
	{
		m_configs.get_ref<config::camara>().increase_fov(e.dy);
		return true;
	}

	mat4 Spinning_Cube::get_model_matrix(const vec3& translate)
	{
		auto init_angle = m_configs.get_ref<config::init_angle>();

		return get_rotation_matrix(init_angle) * get_translate_matrix(translate);
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
				1,0,0,-m_camara_location(0),
				0,1,0,-m_camara_location(1),
				0,0,1,-m_camara_location(2),
				0,0,0,1.f
			}
		};

		return frame * trans;
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

		return get_box_transform_matrix(object_space_box, screen_space_box);
	}


	void Spinning_Cube::compute_view_projection_matrix()
	{

		auto camara_matrix = get_camara_matrix();


		auto m_near = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::near>();
		auto m_far = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::far>();

		auto projection = get_projection_matrix(m_near, m_far);

		auto view_port = view_port_matrix();

		if (m_configs.get_ref<config::perpective>())
		{
			m_vp = view_port * projection * camara_matrix;
			m_projection_matrix = view_port * projection;
		}
		else
		{
			m_vp = view_port *  camara_matrix;
			m_projection_matrix = view_port;
		}

		m_view_matrix = camara_matrix;

	}



	void Spinning_Cube::draw_lines(const std::vector<vec3>& positions, const std::vector<vec2i>& indices, const vec3& color, const mat4& model_matrix)
	{
		m_shader->set_vertex_attribute<Shader::config::pos>(positions);
		//m_shader->set_vertex_attribute<Shader::config::normal>(normals);
		m_shader->set_vertex_attribute<Shader::config::edge_indices>(indices);

		m_shader->set_uniform<Shader::config::view_matrix>(m_view_matrix);
		m_shader->set_uniform<Shader::config::projection_matrix>(m_projection_matrix);
		m_shader->set_uniform<Shader::config::model_matrix>(model_matrix);

		m_shader->set_uniform<Shader::config::light_pos>(m_configs.get_ref<config::light_pos>());
		m_shader->set_uniform<Shader::config::light_color>(m_configs.get_ref<config::light_color>());
		m_shader->set_uniform<Shader::config::ambient>(m_configs.get_ref<config::ambient>());
		m_shader->set_uniform<Shader::config::obj_color>(color);

		m_shader->draw_lines();
	}


	void Spinning_Cube::draw_triangles(const std::vector<vec3>& positions, const std::vector<vec3i>& indices, const std::vector<vec3>& normals, const vec3& color, const mat4& model_matrix)
	{
		m_shader->set_vertex_attribute<Shader::config::pos>(positions);
		m_shader->set_vertex_attribute<Shader::config::normal>(normals);
		m_shader->set_vertex_attribute<Shader::config::indices>(indices);

		m_shader->set_uniform<Shader::config::view_matrix>(m_view_matrix);
		m_shader->set_uniform<Shader::config::projection_matrix>(m_projection_matrix);
		m_shader->set_uniform<Shader::config::model_matrix>(model_matrix);

		m_shader->set_uniform<Shader::config::light_pos>(m_configs.get_ref<config::light_pos>());
		m_shader->set_uniform<Shader::config::light_color>(m_configs.get_ref<config::light_color>());
		m_shader->set_uniform<Shader::config::ambient>(m_configs.get_ref<config::ambient>());
		m_shader->set_uniform<Shader::config::obj_color>(color);

		m_shader->draw_triangles();

	}

	void Spinning_Cube::draw_cubic(const vec3& corner, float side_length, const vec3& color, const mat4& model_matrix)
	{

		std::vector<vec3> cubic_8_pos{ corner };

		std::vector<vec3> space_unit{
			{1.f,0.f,0.f},
			{0.f,1.f,0.f},
			{0.f,0.f,1.f}
		};

		for (int i = 0; i < 3; i++)
		{
			auto backup_pos = cubic_8_pos;
			for (int j = 0; j < backup_pos.size(); j++)
			{
				cubic_8_pos.push_back(backup_pos[j] + side_length * space_unit[i]);
			}
		}

		std::vector<vec4i> cubic_6_side_indices
		{
			{0,1,3,2}, //bottom
			{4,5,7,6}, //up
			{0,2,6,4}, //left
			{1,3,7,5}, //right
			{0,1,5,4}, //front
			{2,3,7,6}, //back
		};

		std::vector<vec3> cubic_6_side_normal
		{
			-space_unit[1], //bottom
			space_unit[1],	// up
			-space_unit[0], //left
			space_unit[0],  // right
			space_unit[2],  //front
			-space_unit[2]  //back
		};


		std::vector<vec3> final_pos(4 * 6);
		std::vector<vec3> final_normal(4 * 6);
		std::vector<vec3i> final_tri(2 * 6);

		for (int si = 0; si < 6; si++)
		{
			for (int svi = 0; svi < 4; svi++)
			{
				final_pos[si * 4 + svi] = cubic_8_pos[cubic_6_side_indices[si](svi)];
				final_normal[si * 4 + svi] = cubic_6_side_normal[si];
			}
			int v[]{ si * 4 + 0,si * 4 + 1,si * 4 + 2,si * 4 + 3 };
			final_tri[si * 2 + 0] = vec3i{ v[0],v[1],v[2] };
			final_tri[si * 2 + 1] = vec3i{ v[0],v[2],v[3] };
		}

		draw_triangles(final_pos, final_tri, final_normal, color, model_matrix);
	}


	void Spinning_Cube::update(int cx, int cy)
	{

		compute_view_projection_matrix();

		m_configs.get_ref<config::camara>().drag(cx, cy);

		auto cube_side = m_configs.get_ref<config::cube_side>();


		if (m_configs.get_ref<config::draw_mesh>())
		{
			const auto& file_name = m_configs.get_ref<config::mesh_file>();

			auto translate = m_configs.get_ref<config::mesh_translate>();
			auto scale = m_configs.get_ref<config::mesh_scale>();
			mat4 model= get_translate_matrix(translate);
			model(0, 0) *= scale;
			model(1, 1) *= scale;
			model(2, 2) *= scale;

			model = model * get_rotation_matrix(m_configs.get_ref<config::init_angle>());

			draw_triangles(
				m_mesh_loader->m_configs.get_ref<Mesh_Loader::config::positions>(),
				m_mesh_loader->m_configs.get_ref<Mesh_Loader::config::indicies>(),
				m_mesh_loader->m_configs.get_ref<Mesh_Loader::config::normals>(),
				{ 1.f,0.f,0.f }, model
			);

			//auto model = get_model_matrix({});
			//draw_cubic({}, cube_side, { 0.5f,0.7f,0.2f }, model);
		}
		
		if (m_configs.get_ref<config::draw_axis>())
		{
			float axis_length = cube_side / 3;

			auto m_near = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::near>();
			auto m_far = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::far>();
			auto m_fov = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::fov>();
			float tangent = std::tan(degree_2_radian(m_fov));
			float half_height = std::abs(m_near) * tangent;
			float half_width = half_height * m_aspect;

			auto toPos = vec3{20,20,m_near};
			auto pv = m_projection_matrix * m_view_matrix;
			auto p = from_homogeneous_point(GS_solve(pv, get_homogeneous_point(toPos)));
			
			auto model = get_translate_matrix(p) * get_scale_matrix({ axis_length,axis_length,axis_length });

			draw_lines({ {0,0,0},  {1,0,0} },
				{ {0,1} },
				{ 1.f,0.f,0.f }, model);

			draw_lines({ {0,0,0},  {0,1,0}  },
				{ {0,1} },
				{ 0.f,1.f,0.f }, model);

			draw_lines({ {0,0,0},  {0,0,1} },
				{ {0,1} },
				{ 0.f,0.f,1.f }, model);
		}

		if (m_configs.get_ref<config::draw_camara>())
		{
			float litte_cube_side = cube_side / 30;

			const auto& lookat = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::lookat>();
			const auto& cam_location = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::location>();
			const auto& cam_up = m_configs.get_ref<config::camara>().m_configs.get_ref<Camara::config::up>();
			vec3 front_dir = normalize(lookat - cam_location);
			vec3 cam_right = normalize(cross(front_dir, cam_up));

			float axis_length = litte_cube_side * 10;

			auto cam_presentation_location = cam_location + 5 * axis_length * front_dir;
			std::vector<vec3> pos
			{
				cam_presentation_location,
				cam_presentation_location + axis_length * cam_right,
				cam_presentation_location + axis_length * cam_up,
				cam_presentation_location - axis_length * front_dir
			};
			std::vector<int> tri{ 0,1,2,0,1,3 };
			std::vector<vec3> tri_color
			{
				{1.f,0.f,0.f} ,
				{0.f,1.f,0.f} ,
			};

			for (int i = 0; i < tri.size() / 3; i++)
			{
				//draw_triangle({ pos[tri[i * 3 + 0]],pos[tri[i * 3 + 1]],pos[tri[i * 3 + 2]] }, tri_color[i], get_identity<float, 4>());
			}

			//draw_triangles({ pos[tri[0 * 3 + 0]],pos[tri[0 * 3 + 1]],pos[tri[0 * 3 + 2]] }, { {0,1,2} }, { front_dir, front_dir, front_dir }, { 1.f,0.f,0.f }, get_identity<float, 4>());

			//draw_lines({ cam_presentation_location,  cam_presentation_location + axis_length * cam_right,cam_presentation_location + axis_length * cam_up ,cam_presentation_location - axis_length * front_dir },
			//	{ {0,1},{0,2},{0,3} },
			//	{ 1.f,0.f,0.f }, get_identity<float, 4>());

		}

		if (m_configs.get_ref<config::draw_light>())
		{
			draw_cubic({}, 10.f, m_configs.get_ref<config::light_color>(), get_translate_matrix(m_configs.get_ref<config::light_pos>()));
		}


		const auto& angle_rate = get_config<config::angle_rate>();
		auto& init_angle = get_config<config::init_angle>();
		init_angle = init_angle + angle_rate;
	}
}
