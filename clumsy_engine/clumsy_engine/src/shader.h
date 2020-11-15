

#pragma once

#include <string>

namespace clumsy_engine
{

    class Shader
    {
        public:
        Shader(const std::string &vertex_src,std::string& fragment_src);

        void bind()const ;

        void unbind()const ;
        private:
        unsigned int m_renderer_id;

    };



}



