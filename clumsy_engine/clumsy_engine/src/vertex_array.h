

#pragma  once

#include "buffer.h"
#include <memory>

namespace clumsy_engine
{


	class  Vertex_Array
	{
	public:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void add_vertex_buffer(std::shared_ptr<Vertex_Buffer> vb)  = 0;
		virtual void set_index_buffer( std::shared_ptr<Index_Buffer> ib)  = 0;

		virtual std::vector<std::shared_ptr<Vertex_Buffer>> get_vertex_buffer() const = 0;
		virtual std::shared_ptr<Index_Buffer> get_index_buffer() const = 0;

		static std::unique_ptr<Vertex_Array> create();
	};

}
