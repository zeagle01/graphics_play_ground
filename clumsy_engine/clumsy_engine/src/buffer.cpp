


#include <memory>
#include "buffer.h"
#include "opengl_buffer.h"
#include "renderer_API.h"
#include "renderer.h"
#include "log.h"

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
}