


#include "vertex_array.h"
#include "renderer.h"
#include "log.h"
#include "openGL_vertex_array.h"




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

}