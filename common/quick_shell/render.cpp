module;

#include "ce_log.h"

#include "glad/glad.h"

#include <string>
#include <array>
#include <memory>

module quick_shell : render;

import :shader;
import :vartex_array;

namespace quick_shell
{
	class renderer_imp
	{
	public:

		void draw_points(const float* pos, int vNum)
		{

			create_shader_if_not_exsit();
			create_vertex_array_if_not_exsit();
			m_points_indies.resize(vNum);
			for (int i = 0; i < vNum; i++)
			{
				m_points_indies[i] = i;
			}

			m_vertex_array->bind();
			m_vertex_array->set_index_buffer(m_points_indies.data(), vNum);
			m_vertex_array->set_vertex_attribute_data(m_attribute_name_position, pos, vNum);

			glEnable(GL_PROGRAM_POINT_SIZE);
			glPointSize(5);

			m_shader->upload_uniform_vec3("u_obj_color", m_point_color);
			draw_iendxes(GL_POINTS);
		}


		void draw_triangles(const int* indices, const float* pos, int tNum, int vNum)
		{

			create_shader_if_not_exsit();

			create_vertex_array_if_not_exsit();

			m_vertex_array->bind();
			m_vertex_array->set_index_buffer(indices, tNum * 3);
			m_vertex_array->set_vertex_attribute_data(m_attribute_name_position, pos, vNum);

			std::array<float, 16> identity
			{
				1.f,0.f,0.f,0.f,
				0.f,1.f,0.f,0.f,
				0.f,0.f,1.f,0.f,
				0.f,0.f,0.f,1.f
			};


			clear_screen();

			m_shader->bind();
			m_shader->upload_uniform_mat4("u_view_projection", identity.data());
			m_shader->upload_uniform_mat4("u_model_matrix", identity.data());

			if (m_is_enable_fill)
			{
				set_polygon_mode(false);
				m_shader->upload_uniform_vec3("u_obj_color", m_fill_color);
				draw_iendxes(GL_TRIANGLES);
			}

			if (m_is_enable_wireframe)
			{
				set_polygon_mode(true);
				m_shader->upload_uniform_vec3("u_obj_color", m_wireframe_color);
				draw_iendxes(GL_TRIANGLES);
			}

		}

		template<typename proc>
		void load_glad(proc* proc_addr)
		{
			if (!gladLoadGLLoader((GLADloadproc)proc_addr)) {
				CE_LOG_INFO("glad load failed\n");
			}
			else {

				CE_LOG_INFO("glad load succend\n");
			}
			printf("gl version %s\n", glGetString(GL_VERSION));

		}

		void enable_fill(bool v) { m_is_enable_fill = v; }
		void enable_wireframe(bool v) { m_is_enable_wireframe = v; }

		void clear_screen()
		{
			glClearColor(0.3, 0.2, 0.1, 1.);//default color;
			glClear(GL_COLOR_BUFFER_BIT);
		}

	private:

		void create_shader_if_not_exsit()
		{
			if (!m_shader)
			{
				//glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				m_shader = std::make_unique<shader>();
				std::string resources_dir = "../../../resources/";
				m_shader->read(resources_dir + "shaders/cloth_sim.glsl");

				m_shader->bind();
				m_shader->upload_uniform_vec3("u_light_pos", {10.f, 10.f, 10.f});
				m_shader->upload_uniform_vec3("u_obj_color", m_fill_color);
				m_shader->upload_uniform_vec3("u_eye_pos", {0.f,0.f,0.f});
				m_shader->upload_uniform_vec3("u_light_color", {1.f, 1.f, 1.f});
				m_shader->upload_uniform_float("u_ka", 0.3f);
				m_shader->upload_uniform_float("u_ks", 0.5f);
				m_shader->upload_uniform_float("u_kd", 0.5f);
				m_shader->upload_uniform_float("u_specular_steep", 40.f);

			}
		}

		void create_vertex_array_if_not_exsit()
		{
			if (!m_vertex_array)
			{
				m_vertex_array = std::make_unique<vertex_array>();
				m_vertex_array->add_vertex_attribute(shader_type_enum::Float3, m_attribute_name_position);
			}
		}


	private:
		void draw_iendxes(int mode)
		{
			glDrawElements(mode, m_vertex_array->get_index_count(), GL_UNSIGNED_INT, nullptr);
		}


		void set_polygon_mode(bool v)
		{
			if (v)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}


	private:
		std::unique_ptr<shader> m_shader;
		std::unique_ptr<vertex_array> m_vertex_array;
		std::vector<int> m_points_indies;
		std::string m_attribute_name_position = "v_position";


		bool m_is_enable_fill = true;
		bool m_is_enable_wireframe = true;
		std::array<float, 3> m_fill_color = { 1.f,0.f,0.f };
		std::array<float, 3> m_wireframe_color = { 0.f,1.f,0.f };
		std::array<float, 3> m_point_color = { 0.f,0.f,1.f };

	};
}
