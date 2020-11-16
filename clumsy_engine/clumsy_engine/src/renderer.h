

#pragma once


namespace clumsy_engine
{

    enum class Renderer_API
    {
        None,
        OpenGL
    };

    class Renderer
    {
        public:
            static inline Renderer_API get_API() { return s_Renderer_API; }

        private:
        static Renderer_API s_Renderer_API;

    };


}