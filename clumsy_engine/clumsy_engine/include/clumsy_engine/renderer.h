

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

            static void begin_scene(std::shared_ptr<Orthorgraphic_Camara> camara);
            static void begin_scene(std::shared_ptr<Orthorgraphic_Camara_NEW> camara);

            static void end_scene();
			static void submit(std::shared_ptr<Shader> shader, std::shared_ptr<Vertex_Array> vertex_array, const glm::mat4& transform);
    private:
        struct SceneData
        {
            glm::mat4 view_projection_matrix;
        };

        static std::shared_ptr<SceneData> s_scene_data;
    };


}