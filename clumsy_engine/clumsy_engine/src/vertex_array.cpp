


#include "vertex_array.h"
#include "renderer.h"
#include "log.h"
#include "openGL_vertex_array.h"
#include "profiler.h"




namespace clumsy_engine
{

	std::unique_ptr<Vertex_Array> Vertex_Array::create()
	{
		switch (Renderer::get_API())
		{
		case Renderer_API::API::None: CE_CORE_WARN("rederer api none!"); return nullptr;
		case Renderer_API::API::OpenGL:  return std::make_unique<OpenGL_Vertex_Array>();
		}

		CE_CORE_ERROR("rederer api error!");
		return nullptr;
	}

	void Vertex_Array::set_positions(float* data, int num)
	{
		RECORD_FUNCTION_DURATION();

		bind();
		get_vertex_buffer()[0]->set_data(data, num);
	}

}