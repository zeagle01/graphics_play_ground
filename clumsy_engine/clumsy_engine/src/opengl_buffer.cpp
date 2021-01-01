


#include "opengl_buffer.h"
#include "glad/glad.h"


namespace clumsy_engine
{

    OpenGL_Vertex_Buffer::OpenGL_Vertex_Buffer(float *vertices, int size)
    {
        glCreateBuffers(1,&m_renderer_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    }


    void OpenGL_Vertex_Buffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER,m_renderer_id);

    }

    void OpenGL_Vertex_Buffer::unbind() const
    {

        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

	void OpenGL_Vertex_Buffer::set_data(float* data, int num)
	{
        bind();
        glBufferData(GL_ARRAY_BUFFER, 3*num * sizeof(float), data, GL_DYNAMIC_DRAW);
	}

    OpenGL_Vertex_Buffer::~OpenGL_Vertex_Buffer()
    {
        glDeleteBuffers(1, &m_renderer_id);
    }






///////////////////////////////

    OpenGL_Index_Buffer::OpenGL_Index_Buffer(int *vertices, int size)
    {
        m_count = size;
        glCreateBuffers(1,&m_renderer_id);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(int), vertices, GL_DYNAMIC_DRAW);
    }

    void OpenGL_Index_Buffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_renderer_id);

    }

    void OpenGL_Index_Buffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGL_Index_Buffer::~OpenGL_Index_Buffer()
    {
        glDeleteBuffers(1, &m_renderer_id);
    }

    int OpenGL_Index_Buffer::get_count() const
    {
        return m_count;
    }
} // namespace clumsy_engine
