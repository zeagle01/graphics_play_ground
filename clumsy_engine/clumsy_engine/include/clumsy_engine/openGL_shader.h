


#pragma once

#include  "shader.h"
#include "glm/glm.hpp"

#include <string>


namespace clumsy_engine
{

	class OpenGL_Shader : public Shader
    {
        public:
        OpenGL_Shader(const std::string &vertex_src,std::string& fragment_src);

		void bind() const override;

		void unbind() const override;

        void upload_uniform_mat4(const std::string& name,const glm::mat4& m);
        void upload_uniform_vec3(const std::string& name,const glm::vec3& m);

        private:
        unsigned int m_renderer_id;

    };

}

