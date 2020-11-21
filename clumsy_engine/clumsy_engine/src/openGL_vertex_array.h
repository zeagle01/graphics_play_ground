


#pragma once

#include "vertex_array.h"


namespace clumsy_engine
{


	class OpenGL_Vertex_Array :public Vertex_Array
	{
	public:
		OpenGL_Vertex_Array();
		~OpenGL_Vertex_Array();
		 void bind() const override;
		 void unbind() const  override;

		 void add_vertex_buffer(std::shared_ptr<Vertex_Buffer> vb) override;
		 void set_index_buffer(std::shared_ptr<Index_Buffer> ib) override;

		 std::vector<std::shared_ptr<Vertex_Buffer>> get_vertex_buffer() const override { return m_vertex_buffers; }
		 std::shared_ptr<Index_Buffer> get_index_buffer() const override{ return m_index_buffer; };

	private:
		std::vector<std::shared_ptr<Vertex_Buffer>> m_vertex_buffers;
		std::shared_ptr<Index_Buffer> m_index_buffer;
		unsigned int m_renderer_id;
	};

}