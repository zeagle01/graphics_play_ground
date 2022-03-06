

#pragma  once

#include "buffer.h"
#include "ref.h"

#include <memory>
#include <string>

namespace clumsy_engine
{


	class Vertex_Array
	{
	public:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void set_index_buffer(Ref<Index_Buffer> ib)  = 0;
		virtual Ref<Index_Buffer> get_index_buffer() const = 0;

		virtual void add_vertex_attribute( Shader_Data_Type data_type, const std::string& name_in_shader) = 0;
		virtual void set_vertex_attribute_data(const std::string& name_in_shader, const float* data, int count) = 0;


		static std::unique_ptr<Vertex_Array> create();
	};

	////TODO:
	//class Seperate_Layout_Vertex_Array:public Vertex_Array
	//{
	//public:
	//};

}
