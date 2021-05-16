

#pragma  once

#include "buffer.h"
#include "ref.h"

#include <memory>

namespace clumsy_engine
{


	class  Vertex_Array
	{
	public:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void add_vertex_buffer(Ref<Vertex_Buffer> vb)  = 0;
		virtual void set_index_buffer( Ref<Index_Buffer> ib)  = 0;

		virtual std::vector<Ref<Vertex_Buffer>> get_vertex_buffer() const = 0;
		virtual Ref<Index_Buffer> get_index_buffer() const = 0;

		void set_positions(float* data, int num);

		static std::unique_ptr<Vertex_Array> create();
	};

}
