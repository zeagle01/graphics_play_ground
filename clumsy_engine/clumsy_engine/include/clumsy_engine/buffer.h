

#pragma once

#include <memory>
#include <vector> 
#include <string>
#include "glad/glad.h"

namespace clumsy_engine
{

    enum class Shader_Data_Type
    {
        Bool,
        Int,
        Int2,
        Int3,
        Int4,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4
    };

	template<Shader_Data_Type sdt, typename T,int gl_T, int count >
    struct Shader_Data_Type_Traits 
    {
		using type = T;
		static  constexpr Shader_Data_Type enum_type = sdt;
		static  constexpr int count = count;
		static  constexpr int gl_type = gl_T;
    };

	template<Shader_Data_Type sdt>
    struct Shader_Data_Type_Record;

#define DEF_Shader_Data_Type_Record(x,t,gl_t,n) \
    template<> struct Shader_Data_Type_Record<Shader_Data_Type::x>: public Shader_Data_Type_Traits<Shader_Data_Type::x, t, gl_t, n>{};\

	DEF_Shader_Data_Type_Record(Bool, bool, GL_BOOL, 1)
	DEF_Shader_Data_Type_Record(Int, int, GL_INT, 1)
	DEF_Shader_Data_Type_Record(Int2, int, GL_INT, 2)
	DEF_Shader_Data_Type_Record(Int3, int, GL_INT, 3)
	DEF_Shader_Data_Type_Record(Int4, int, GL_INT, 4)
	DEF_Shader_Data_Type_Record(Float, float, GL_FLOAT, 1)
	DEF_Shader_Data_Type_Record(Float2, float, GL_FLOAT, 2)
	DEF_Shader_Data_Type_Record(Float3, float, GL_FLOAT, 3)
	DEF_Shader_Data_Type_Record(Float4, float, GL_FLOAT, 4)
	DEF_Shader_Data_Type_Record(Mat3, float, GL_FLOAT, 3*3)
	DEF_Shader_Data_Type_Record(Mat4, float, GL_FLOAT, 4*4)


    struct Buffer_Element
    {
        std::string name;
        Shader_Data_Type type;
        int count;
        int size;
        int offset;
        int gl_type;
        bool normalized;
		Buffer_Element(Shader_Data_Type type, std::string name, bool normalized = false)
            :
            name(name),
            type(type),
            size(0),
            offset(0),
            gl_type(0),
			normalized(normalized)
        {
        }
        
    };

    template<typename E,E v>
    struct collect_element_layout
    {
        static void apply(Buffer_Element& elements,int& offset,int& stride)
		{
            if (elements.type==v)
            {
				elements.offset = offset;
                elements.count = Shader_Data_Type_Record<v>::count;
				elements.size = sizeof(Shader_Data_Type_Record<v>::type) * elements.count;
				elements.gl_type = Shader_Data_Type_Record<v>::gl_type;

				offset += elements.size;
				stride += elements.size;
            }
		}
    };

    class Buffer_Layout
    {
    public:
        Buffer_Layout() {}
		Buffer_Layout(const std::initializer_list<Buffer_Element>& elements) :m_elements(elements) 
        {
            calculate_offset_and_stride();
        }

        inline const std::vector<Buffer_Element>& get_elements() const { return m_elements; }

		int get_stride()const { return m_stride; }

        auto begin() { return m_elements.begin(); }
        auto end() { return m_elements.end(); }

		auto begin()const { return m_elements.cbegin(); }
		auto end() const { return m_elements.cend(); }
    private:
        void calculate_offset_and_stride();
        
    private:
        std::vector<Buffer_Element> m_elements;
        int m_stride;

    };

    class Vertex_Buffer
    {
        public:
            virtual ~Vertex_Buffer() {}
            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual const Buffer_Layout& get_layout()const = 0;
            virtual void set_layout(const Buffer_Layout& layout) = 0;

            virtual void set_data(float const* data,int num) = 0;

            static std::unique_ptr<Vertex_Buffer> create(float *vertices, int size);
            static std::unique_ptr<Vertex_Buffer> create();
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