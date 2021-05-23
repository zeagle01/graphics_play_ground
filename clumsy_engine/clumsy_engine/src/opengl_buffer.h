



#pragma once
#include "buffer.h"


namespace clumsy_engine
{


    class OpenGL_Vertex_Buffer:public  Vertex_Buffer
    {
        public:
            OpenGL_Vertex_Buffer(float *vertices, int size);
            ~OpenGL_Vertex_Buffer();

            void bind() const override;
            void unbind() const override;

            const Buffer_Layout& get_layout()const override
            {
                return m_layout;

            }
            void set_layout(const Buffer_Layout& layout) override
            {
                m_layout = layout;
            }
			void set_data(float const * data, int num)override;

        private:
            unsigned int m_renderer_id;
            Buffer_Layout m_layout;


    };

    class OpenGL_Index_Buffer:public  Index_Buffer
    {
        public:
            OpenGL_Index_Buffer(int *vertices, int size);

            ~OpenGL_Index_Buffer();

            void bind() const override;
            void unbind() const override;



            int get_count() const override;

        private:
            unsigned int m_renderer_id;
            unsigned int m_count;



    };

}