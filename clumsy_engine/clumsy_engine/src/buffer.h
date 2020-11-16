

#pragma once

#include <memory>

namespace clumsy_engine
{

    class Vertex_Buffer
    {
        public:
            virtual ~Vertex_Buffer() {}
            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            //virtual void set_data() = 0;

            static std::unique_ptr<Vertex_Buffer> create(float *vertices, int size);
    };


    class Index_Buffer
    {
        public:
            virtual ~Index_Buffer() {};
            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual int get_count() const = 0;

            //virtual void set_data() = 0;

            static std::unique_ptr<Index_Buffer> create(int *indices, int size);
    };

}