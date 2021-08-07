


#pragma once

#include "vertex_array.h"
#include <map>
#include <string>


namespace clumsy_engine
{


	class OpenGL_Vertex_Array :public Vertex_Array
	{
	public:
		OpenGL_Vertex_Array();
		~OpenGL_Vertex_Array();
		 void bind() const override;
		 void unbind() const  override;

		 void set_index_buffer(Ref<Index_Buffer> ib) override;

		 void add_vertex_attribute(int shader_program, Shader_Data_Type data_type,const std::string& name_in_shader);
		 void set_vertex_attribute_data(const std::string& name_in_shader, const float* data,int count);


		 Ref<Index_Buffer> get_index_buffer() const override{ return m_index_buffer; };

	private:
		void add_vertex_buffer(Ref<Vertex_Buffer> vb, int shader_program);

	private:
		Ref<Index_Buffer> m_index_buffer;
		unsigned int m_renderer_id;
		std::map<std::string, Ref<Vertex_Buffer>> m_vertex_buffers;
	};

}