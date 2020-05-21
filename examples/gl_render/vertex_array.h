



#pragma once

#include "vertex_buffer_layout.h"
#include "vertex_buffer.h"


class Vertex_Array
{

private:
	unsigned int m_render_id;

public:
	Vertex_Array();

	void add_buffer(const Vertex_Buffer& buffer, const Vertex_Buffer_Layout& buffer_layout);
	void bind()const;
	void unbind() const;

};