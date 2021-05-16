

#pragma once

#include "renderer_API.h"
#include "glm/glm.hpp"

namespace clumsy_engine
{

    class Orthorgraphic_Camara;
    class Orthorgraphic_Camara_NEW;
    class Shader;

    class Renderer
    {
        public:
			static inline Renderer_API::API get_API() { return Renderer_API::get_API(); }

            static void begin_scene(Ref<Orthorgraphic_Camara> camara);
            static void begin_scene(Ref<Orthorgraphic_Camara_NEW> camara);

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