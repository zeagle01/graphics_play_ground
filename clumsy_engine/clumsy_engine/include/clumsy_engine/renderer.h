

#pragma once

#include "renderer_API.h"
#include "glm/glm.hpp"

namespace clumsy_engine
{

    class Camara;
    class Shader;

    class Renderer
    {
        public:
			static inline Renderer_API::API get_API() { return Renderer_API::get_API(); }

            static void begin_scene(Ref<Camara> camara);

            static void end_scene();
			static void submit(Ref<Shader> shader, Ref<Vertex_Array> vertex_array, const glm::mat4& transform);
    private:
        struct SceneData
        {
            glm::mat4 view_projection_matrix;
        };

        static Ref<SceneData> s_scene_data;
    };


}