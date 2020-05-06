

#include "index_buffer.h"

#include "glad/glad.h"

#include "renderer.h"


Index_Buffer::Index_Buffer(const unsigned int* data, unsigned int element_num) :m_element_num(element_num)
{

	GL_Call(glGenBuffers(1, &m_renderer_id));
	GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_renderer_id));

	GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_num * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));


}

Index_Buffer::~Index_Buffer()
{
	glDeleteBuffers(1,&m_renderer_id);
}

void Index_Buffer::bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void Index_Buffer::unbind()const 
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Index_Buffer::set_data(const unsigned int* data, unsigned int element_num)
{
	bind();
	GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_num * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
}
