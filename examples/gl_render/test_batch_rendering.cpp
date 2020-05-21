#include "test_batch_rendering.h"
#include "glog/logging.h"

#include "glad/glad.h"
#include "renderer.h"
#include "imgui.h"

#include <cstddef>


namespace test
{

	Test_Batch_Rendering::Test_Batch_Rendering()
		:m_is_gpu_allocated(false)
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


		GL_Call(glGenBuffers(1, &m_ebo));
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));




		LOG(INFO) << "Test_Batch_Rendering " << (void*)this << " construct";
	}
	Test_Batch_Rendering::~Test_Batch_Rendering()
	{
		LOG(INFO) << "Test_Batch_Rendering " << (void*)this << " destruct";
	}

	void Test_Batch_Rendering::on_update(float deltaTime)
	{

	}

	float color_clamp(float value)
	{
		if (value>1.0)
		{
			value = value - 1.0;

		}
		else if(value<0.)
		{
			value += 1.0;
		}
		return value;

	}
	void Test_Batch_Rendering::on_render()
	{
		if (!m_is_gpu_allocated)
		{
			m_vertices =
			{
				{{-0.5f,-0.5f,0.0f},{0.2f,0.f,1.f,1.f}},
				{{0.5f,-0.5f,1.0f},{0.f,0.5f,1.f,1.f}},
				{{0.5f,0.5f,1.0f},{0.1f,0.6f,1.f,1.f}},
				{{-0.5f,0.5f,0.0f},{0.9f,0.f,0.9f,1.f}}
			};
			
			m_indices =
			{
				0,1,2,
				2,3,0
			};

			GL_Call(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));
			GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));

			m_translate[0] = 0.;
			m_translate[1] = 0.;
			m_translate[2] = 0.;

			m_color_translate[0] = 0;
			m_color_translate[1] = 0;
			m_color_translate[2] = 0;

			m_is_gpu_allocated = true;;
		}
		for (auto& vertex : m_vertices)
		{
			vertex.position[0] += m_translate[0];
			vertex.position[1] += m_translate[1];
			vertex.position[2] += m_translate[2];

			vertex.color[0] = color_clamp(vertex.color[0] + m_color_translate[0]);
			vertex.color[1] = color_clamp(vertex.color[1] + m_color_translate[1]);
			vertex.color[2] = color_clamp(vertex.color[2] + m_color_translate[2]);
		}
		LOG(INFO) << m_vertices[0].color[0];

		GL_Call(glBindBuffer(GL_ARRAY_BUFFER,m_vbo));
		GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));

//		GL_Call(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW));
//		GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_DYNAMIC_DRAW));
		GL_Call(glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Vertex)*m_vertices.size(),&m_vertices[0]));
		GL_Call(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,sizeof(unsigned int)*m_indices.size(),&m_indices[0]));

		GL_Call(glBindVertexArray(m_vao));
		GL_Call(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr));

	}
	void Test_Batch_Rendering::on_imgui_render()
	{

		ImGui::SliderFloat3("translate", m_translate, -0.01f, .01f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat3("color translate", m_color_translate, -1e-2f, 1e-2f);            // Edit 1 float using a slider from 0.0f to 1.0f
	}
}

