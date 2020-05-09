

#include "vertex_buffer.h"

#include "glad/glad.h"

#include "renderer.h"


Vertex_Buffer::Vertex_Buffer(const void* data, unsigned int size)
{

	GL_Call(glGenBuffers(1, &m_renderer_id));
	GL_Call(glBindBuffer(GL_ARRAY_BUFFER,m_renderer_id));

	GL_Call(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_DYNAMIC_DRAW));


}

Vertex_Buffer::~Vertex_Buffer()
{
	glDeleteBuffers(1,&m_renderer_id);
}

void Vertex_Buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void Vertex_Buffer::unbind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Vertex_Buffer::set_data(const void* data, unsigned int size)
{

	bind();
	GL_Call(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_DYNAMIC_DRAW));
}
