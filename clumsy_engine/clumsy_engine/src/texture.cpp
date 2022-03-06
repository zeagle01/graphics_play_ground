

#include "texture.h"
#include "renderer_API.h"
#include "renderer.h"
#include "log.h"
#include "openGL_texture.h"



namespace clumsy_engine
{
	Ref<Texture_2D> Texture_2D::create(const std::string& path)
	{
		switch (Renderer::get_API())
		{
		case Renderer_API::API::None: CE_CORE_WARN("rederer api none!"); return nullptr;
		case Renderer_API::API::OpenGL:  return std::make_unique<OpenGL_Texture_2D>(path);
		}

		CE_CORE_ERROR("rederer api error!");
		return nullptr;
	}

	Ref<Texture_2D> Texture_2D::create(int width, int height)
	{

		switch (Renderer::get_API())
		{
			case Renderer_API::API::None: CE_CORE_WARN("rederer api none!"); return nullptr;
			case Renderer_API::API::OpenGL:  return std::make_unique<OpenGL_Texture_2D>(width, height);
		}

		CE_CORE_ERROR("rederer api error!");
		return nullptr;

	}
}
