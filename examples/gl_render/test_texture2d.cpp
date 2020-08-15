
#include "test_texture2d.h"
#include "glog/logging.h"
#include "glad/glad.h"
#include "renderer.h"
#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	Test_Texture2D::Test_Texture2D()
	{
		LOG(INFO) << "test texture 2d "<<(void*)this<<" created ";

		m_positions =
		{
			-0.5f,-0.5f,0.0f,0.0f,
			0.5f,-0.5f,1.0f,0.0f,

			0.5f,0.5f,1.0f,1.0f,
			-0.5f,0.5f,0.0f,1.0f
		};


		m_indices =
		{
			0,1,2,
			2,3,0
		};

		m_uniform_color = { 1,0,0,1 };

		GL_Call(glEnable(GL_BLEND));
		GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		std::string resources_dir = "../../../resources/";

		m_shader.create_shader_from_file(resources_dir + "shaders/gl_render.vs", resources_dir + "shaders/gl_render.fs");
		m_shader.bind();

		m_vbo = std::make_unique< Vertex_Buffer>(m_positions.data(), m_positions.size());
		m_ibo = std::make_unique<Index_Buffer>(m_indices.data(), m_indices.size());

		m_va = std::make_unique<Vertex_Array>();
		Vertex_Buffer_Layout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		m_va->add_buffer(*m_vbo, layout);

		m_texture = std::make_unique<Texture>(resources_dir + "textures/awesomeface.png");
		int active_texture_slot = 0;
		m_texture->bind(active_texture_slot);
		m_shader.set_uniform_1i("u_texture", active_texture_slot);



		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		m_frame = 0;


		m_proj = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);
		m_view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0, 0));
		m_model_translates =
		{
			glm::vec3(0.8f, 0.9f, 0),
			glm::vec3(0.0f, 0.2f, 0),
			glm::vec3(-0.8f, -0.9f, 0)
		};
	}

	Test_Texture2D::~Test_Texture2D()
	{
		LOG(INFO) << "test texture 2d "<<(void*)this<<" delete ";
	}

	void Test_Texture2D::on_update(float deltaTime)
	{
		float f = 0.1;
		float a = 0.02;
		for (size_t i = 0; i < m_positions.size() / 3; i++)
		{
			m_positions[i * 2 + 0] += a * std::sin(m_frame * f);
		}
		m_indices[0] = m_frame % 2;

		for (size_t i = 0; i < m_uniform_color.size(); i++)
		{
			m_uniform_color[i] = (std::sin(m_frame * f + i) + 1.) * 0.5 + 0.2f;
		}



		m_shader.set_uniform_4f("u_color", m_uniform_color[0], m_uniform_color[1], m_uniform_color[2], m_uniform_color[3]);




		m_va->bind();
		m_vbo->bind();
		m_vbo->set_data(m_positions.data(), m_positions.size());
		m_ibo->set_data(m_indices.data(), m_indices.size());

		m_frame++;
	}
	void Test_Texture2D::on_render()
	{
		Renderer renderer;
		//renderer.clear();

		for (int i = 0; i < m_model_translates.size(); i++)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_model_translates[i]);
			glm::mat4 mvp = m_proj * m_view * model;
			m_shader.set_uniform_mat4f("u_MVP", &mvp[0][0]);


			m_va->bind();
			m_vbo->bind();
			m_vbo->set_data(m_positions.data(), m_positions.size());
			m_ibo->set_data(m_indices.data(), m_indices.size());
			renderer.draw(*m_va, *m_ibo, m_shader);
		}

	}
	void Test_Texture2D::on_imgui_render()
	{
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGui::SliderFloat3("model translate", &m_model_translates[0].x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat4("uniform color", &m_uniform_color[0], 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

	}

}
