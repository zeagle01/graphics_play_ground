#include "test_batch_rendering.h"
#include "glog/logging.h"

#include "glad/glad.h"
#include "renderer.h"
#include "imgui.h"

#include <cstddef>


namespace test
{
	void Test_Batch_Rendering::push_quad(const Quad& quad)
	{
		int v_begin = m_vertices.size();
		for (int i = 0; i < 4; i++)
		{
			m_vertices.push_back(quad.vertices[i]);
		}
		for (int i = 0; i < 6; i++)
		{
			m_indices.push_back(quad.indices[i] + v_begin);
		}

	}

	Test_Batch_Rendering::Test_Batch_Rendering()
		:m_is_gpu_allocated(false), 
		m_translate { 0, 0, 0 },
		m_color_translate{ 0,0,0 },
		m_texture_cood_translate{ 0,0 },
		m_row(3),
		m_col(3)
	{

		m_shader = std::make_unique<Shader>();
		m_shader->create_shader_from_file("cases/batch_rendering.vs", "cases/batch_rendering.fs");

		m_shader->bind();
		
		//construct  

		GL_Call(glGenVertexArrays(1, &m_vao));
		GL_Call(glBindVertexArray(m_vao));

		GL_Call(glGenBuffers(1, &m_vbo));
		GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

		GL_Call(glEnableVertexAttribArray(0));
		GL_Call(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(const void*)offsetof(Vertex,position)));

		GL_Call(glEnableVertexAttribArray(1));
		GL_Call(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),(const void*)offsetof(Vertex,color)));

		GL_Call(glEnableVertexAttribArray(2));
		GL_Call(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(const void*)offsetof(Vertex,texture_cood)));

		GL_Call(glEnableVertexAttribArray(3));
		GL_Call(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),(const void*)offsetof(Vertex,texture_unit)));


		GL_Call(glGenBuffers(1, &m_ebo));
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));

		float scale = 0.2f;
		float stride = 0.25f;
		for (size_t i = 0; i < m_col; i++)
		{
			for (size_t j = 0; j < m_row; j++)
			{
				push_quad(Quad{ i * stride,j * stride,scale,scale,float((i + j) % 2) });
			}
		}

		m_textures.push_back(std::make_unique<Texture>("resources/textures/awesomeface.png"));
		m_textures.push_back(std::make_unique<Texture>("resources/textures/red_square.png"));

		for (size_t i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->bind(i);
		}
		std::vector< int> samplers{ 0,1 };
		m_shader->set_uniform_1iv("u_textures", 2,&samplers[0]);



		LOG(INFO) << "Test_Batch_Rendering " << (void*)this << " construct";
	}
	Test_Batch_Rendering::~Test_Batch_Rendering()
	{
		LOG(INFO) << "Test_Batch_Rendering " << (void*)this << " destruct";
	}

	void Test_Batch_Rendering::on_update(float deltaTime)
	{

	}

	float cirle_clamp(float value,float min,float max)
	{
		if (value > max)
		{
			value = min + (value - max);
		}
		else if (value < min)
		{
			
			value = max - (min - value);
		}
		return value;

	}
	void Test_Batch_Rendering::on_render()
	{
		if (!m_is_gpu_allocated)
		{

			GL_Call(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));
			GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));


			m_is_gpu_allocated = true;;
		}
		for (auto& vertex : m_vertices)
		{
			vertex.position[0] = cirle_clamp(vertex.position[0] + m_translate[0], -1, 1);
			vertex.position[1] = cirle_clamp(vertex.position[1] + m_translate[1], -1, 1);
			vertex.position[2] = cirle_clamp(vertex.position[2] + m_translate[2], -1, 1);

			vertex.color[0] = cirle_clamp(vertex.color[0] + m_color_translate[0],0,1);
			vertex.color[1] = cirle_clamp(vertex.color[1] + m_color_translate[1],0,1);
			vertex.color[2] = cirle_clamp(vertex.color[2] + m_color_translate[2],0,1);

			vertex.texture_cood[0] = cirle_clamp(vertex.texture_cood[0] * (1.0+m_texture_cood_translate[0]),0,1);
			vertex.texture_cood[1] = cirle_clamp(vertex.texture_cood[1] * (1.0+m_texture_cood_translate[1]),0,1);
		}


		for (size_t i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->bind(i);
		}
		std::vector< int> samplers{ 0,1 };
		m_shader->set_uniform_1iv("u_textures", 2,&samplers[0]);


		GL_Call(glBindBuffer(GL_ARRAY_BUFFER,m_vbo));
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));

		GL_Call(glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Vertex)*m_vertices.size(),&m_vertices[0]));
		GL_Call(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,sizeof(unsigned int)*m_indices.size(),&m_indices[0]));

		GL_Call(glBindVertexArray(m_vao));
		GL_Call(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr));

	}
	void Test_Batch_Rendering::on_imgui_render()
	{

		ImGui::SliderFloat3("translate", m_translate, -0.01f, .01f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat3("color translate", m_color_translate, -1e-2f, 1e-2f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat2("texture_cood translate", m_texture_cood_translate, -1e-3f, 1e-3f);            // Edit 1 float using a slider from 0.0f to 1.0f
	}
}

