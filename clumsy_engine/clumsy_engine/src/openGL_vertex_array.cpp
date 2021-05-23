

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

	void OpenGL_Vertex_Array::add_vertex_buffer(Ref<Vertex_Buffer> vb, int shader_program)
	{
		if (vb->get_layout().get_elements().empty())
		{
			CE_CORE_ERROR("vertex buffer got no layout yet");
		}


		glBindVertexArray(m_renderer_id);

		vb->bind();
		const auto& layout = vb->get_layout();
		int index= 0;
		for (const auto& e : layout)
		{
			auto location=glGetAttribLocation(shader_program, e.name.c_str());

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, e.count, e.gl_type, e.normalized, layout.get_stride(), (void*)e.offset);
			index++;
		}

		m_vertex_buffers.push_back(vb);


	}

	void OpenGL_Vertex_Array::set_index_buffer(Ref<Index_Buffer> ib) 
	{
		glBindVertexArray(m_renderer_id);
		ib->bind();
		m_index_buffer = ib;
	}

}