

#include "openGL_vertex_array.h"
#include "glad/glad.h"
#include "buffer.h"
#include "log.h"


namespace clumsy_engine
{

	OpenGL_Vertex_Array::OpenGL_Vertex_Array()
	{
		glCreateVertexArrays(1, &m_renderer_id);
	}

	OpenGL_Vertex_Array::~OpenGL_Vertex_Array()
	{
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void OpenGL_Vertex_Array::bind() const
	{
		glBindVertexArray(m_renderer_id);

	}
	void OpenGL_Vertex_Array::unbind() const
	{

		glBindVertexArray(0);
	}

	void OpenGL_Vertex_Array::add_vertex_attribute(Shader_Data_Type data_type, const std::string& name_in_shader)
	{
		if (m_vertex_buffers.count(name_in_shader) && m_vertex_buffers[name_in_shader] != nullptr)
		{
			CE_CORE_ERROR("already have a given name vbo");
			return;
		}

		clumsy_engine::Ref<clumsy_engine::Vertex_Buffer > vbo = clumsy_engine::Vertex_Buffer::create();
		clumsy_engine::Buffer_Layout layout = { {data_type, name_in_shader} };
		vbo->set_layout(layout);
		m_vertex_buffers[name_in_shader] = vbo;
		add_vertex_buffer(vbo);
	}

	void OpenGL_Vertex_Array::set_vertex_attribute_data(const std::string& name_in_shader, const float* data,int count)
	{
		if (m_vertex_buffers.count(name_in_shader))
		{
			bind();
			auto vbo = m_vertex_buffers[name_in_shader];
			auto layout = vbo->get_layout().get_elements()[0];
			int size_in_byte = layout.size * count;
			vbo->set_data(data, size_in_byte);
		}
		else
		{
			CE_CORE_ERROR("can't find vertex buffer in vertex array ");
		}
	}



	void OpenGL_Vertex_Array::add_vertex_buffer(Ref<Vertex_Buffer> vb)
	{
		if (vb->get_layout().get_elements().empty())
		{
			CE_CORE_ERROR("vertex buffer got no layout yet");
		}

		glBindVertexArray(m_renderer_id);

		vb->bind();

		const auto& layout = vb->get_layout();
		for (const auto& e : layout) //make sure consistent with shader
		{
			glEnableVertexAttribArray(m_location_in_shader);
			glVertexAttribPointer(m_location_in_shader, e.count, e.gl_type, e.normalized, layout.get_stride(), (void*)e.offset);
			m_location_in_shader++;
		}
	}

	void OpenGL_Vertex_Array::set_index_buffer(Ref<Index_Buffer> ib) 
	{
		glBindVertexArray(m_renderer_id);
		ib->bind();
		m_index_buffer = ib;
	}

}