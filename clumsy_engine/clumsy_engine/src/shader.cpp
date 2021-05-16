


#include "shader.h"
#include "openGL_shader.h"
#include "renderer.h"
#include "log.h"
#include "openGL_vertex_array.h"
#include "profiler.h"


namespace clumsy_engine
{

	std::unique_ptr<Shader> Shader::create(const std::string &vertex_src,std::string& fragment_src)
	{
		switch (Renderer::get_API())
		{
		case Renderer_API::API::None: CE_CORE_WARN("rederer api none!"); return nullptr;
		case Renderer_API::API::OpenGL:  return std::make_unique<OpenGL_Shader>(vertex_src, fragment_src);
		}

		CE_CORE_ERROR("Shader create error!");
		return nullptr;
	}

}  