


#include "vertex_array.h"  
#include "vertex_buffer.h"
#include "glad/glad.h"
#include "renderer.h"
#include "glog/logging.h"

void Vertex_Array::add_buffer(const Vertex_Buffer& buffer, const Vertex_Buffer_Layout& buffer_layout)
{
	bind();

	buffer.bind();

	const auto& elements = buffer_layout.get_elements();
	unsigned int offset = 0;
	for (size_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		GL_Call(glEnableVertexAttribArray(i));
		GL_Call(glVertexAttribPointer(i, element.count, element.type, element.normalize, buffer_layout.get_stride(),(void*)offset));
		offset += element.count*Vertex_Buffer_Element::get_size_of_type(element.type);
	}

}


Vertex_Array::~Vertex_Array()
{
	LOG(INFO) << "Vertex_Array " << (void*)this << "destruct!";
}

Vertex_Array::Vertex_Array()
{
	GL_Call(glGenVertexArrays(1, &m_render_id));
	LOG(INFO) << "Vertex_Array " << (void*)this << "construct!";
}
void Vertex_Array::bind()const
{

	GL_Call(glBindVertexArray(m_render_id));

}

void Vertex_Array::unbind() const
{
	GL_Call(glBindVertexArray(0));
}
