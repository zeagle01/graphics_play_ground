

#pragma once

#include <string>
#include "glm/glm.hpp"


namespace clumsy_engine
{

    class Shader
    {
        public:
        Shader(const std::string &vertex_src,std::string& fragment_src);

        void bind()const ;

        void unbind()const ;

        void upload_uniform_mat4(const std::string& name,const glm::mat4& m);

        private:
        unsigned int m_renderer_id;

    };



}



