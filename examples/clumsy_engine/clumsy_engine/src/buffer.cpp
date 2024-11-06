


#include <memory>
#include "buffer.h"
#include "opengl_buffer.h"
#include "renderer_API.h"
#include "renderer.h"
#include "log.h"
#include "enum_loop.h"

namespace clumsy_engine
{
    std::unique_ptr<Vertex_Buffer> Vertex_Buffer::create(float *vertices, int size)
    {
        switch (Renderer::get_API())
        {
        case Renderer_API::API::None:
            CE_CORE_ERROR("none api");
            return nullptr;
            break;
        case Renderer_API::API::OpenGL:
            return std::make_unique<OpenGL_Vertex_Buffer>(vertices, size);
            break;
        }
        CE_CORE_ERROR("no api case");
        return nullptr;
    }

	std::unique_ptr<Vertex_Buffer> Vertex_Buffer::create(int size)
    {
        switch (Renderer::get_API())
        {
        case Renderer_API::API::None:
            CE_CORE_ERROR("none api");
            return nullptr;
            break;
        case Renderer_API::API::OpenGL:
			return std::make_unique<OpenGL_Vertex_Buffer>(size);
            break;
        }
        CE_CORE_ERROR("no api case");
        return nullptr;
    }

    std::unique_ptr<Vertex_Buffer> Vertex_Buffer::create()
    {
        switch (Renderer::get_API())
        {
        case Renderer_API::API::None:
            CE_CORE_ERROR("none api");
            return nullptr;
            break;
        case Renderer_API::API::OpenGL:
            return std::make_unique<OpenGL_Vertex_Buffer>();
            break;
        }
        CE_CORE_ERROR("no api case");
        return nullptr;
    }

    std::unique_ptr<Index_Buffer> Index_Buffer::create(int *indices, int size)
    {
        switch (Renderer::get_API())
        {
        case Renderer_API::API::None:
            CE_CORE_ERROR("none api");
            return nullptr;
            break;
        case Renderer_API::API::OpenGL:
            return std::make_unique<OpenGL_Index_Buffer>(indices, size);
            break;
        }
        CE_CORE_ERROR("no api case");
        return nullptr;

    }

	void Buffer_Layout::calculate_offset_and_stride()
	{
		int offset = 0;
		m_stride = 0;
		for (auto& e : m_elements)
		{
			loop_enum<Shader_Data_Type>::apply<collect_element_layout>(e, offset, m_stride);
		}
	}
}