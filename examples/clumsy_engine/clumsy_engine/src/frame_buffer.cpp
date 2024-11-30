

#include "frame_buffer.h"

#include "renderer.h"
#include "log.h"

#include "openGL_frame_buffer.h"

namespace clumsy_engine
{

	Ref<frame_buffer> frame_buffer::create(const specification& specs)
	{

		switch (Renderer::get_API())
		{
		case Renderer_API::API::None: CE_CORE_WARN("rederer api none!"); return nullptr;
		case Renderer_API::API::OpenGL:  return std::make_shared<openGL_frame_buffer>(specs);
		}

		CE_CORE_ERROR("rederer api error!");
		return nullptr;
	}
}