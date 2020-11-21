

#pragma once

#include "renderer_API.h"

namespace clumsy_engine
{


    class Renderer
    {
        public:
			static inline Renderer_API::API get_API() { return Renderer_API::get_API(); }

            static void begin_scene();
            static void end_scene();
            static void submit(std::shared_ptr<Vertex_Array> vertex_array);

    };


}