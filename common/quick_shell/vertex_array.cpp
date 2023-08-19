module;

#include "ce_log.h";

#include "glad/glad.h"

#include <string>
#include <map>

module main_window:vartex_array;


namespace quick_shell
{
	enum class shader_type_enum
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

	template<shader_type_enum sdt, typename T,int gl_T, int count >
    struct shader_type_enum_traits 
    {
		using type = T;
		static  constexpr shader_type_enum enum_type = sdt;
		static  constexpr int count = count;
		static  constexpr int gl_type = gl_T;
    };

	template<shader_type_enum sdt>
    struct shader_type_enum_record;

#define DEF_Shader_Data_Type_Record(x,t,gl_t,n) \
    template<> struct shader_type_enum_record<shader_type_enum::x>: public shader_type_enum_traits<shader_type_enum::x, t, gl_t, n>{};\

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



    class buffer_layout;

	//vbo
	class vertex_buffer
	{
	public:
		vertex_buffer()
		{
			glCreateBuffers(1, &m_renderer_id);
		}

		~vertex_buffer()
		{
			glDeleteBuffers(1, &m_renderer_id);
		}

		void bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		};

		void unbind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		};

		const buffer_layout& get_layout() { return *m_layout; };

		void set_layout(std::unique_ptr< buffer_layout> layout) {

			m_layout = std::move(layout);
		} 

		void set_data(float const* data, int size_in_bytes)
		{
			bind();
			glBufferData(GL_ARRAY_BUFFER, size_in_bytes, data, GL_DYNAMIC_DRAW);
		}
	private:
		uint32_t m_renderer_id;
		std::unique_ptr<buffer_layout> m_layout;
	};

    struct buffer_element
    {
		std::string name;
		shader_type_enum type;
		int count;
		int size = 0;
		int offset = 0;
		int gl_type = 0;
		bool normalized = false;
    };


    class buffer_layout
    {
    public:
		void add_buffer_elements(const std::initializer_list<buffer_element>& elements)
		{
			m_elements = elements;
            calculate_offset_and_stride();
		}
        inline const std::vector<buffer_element>& get_elements() const { return m_elements; }

		int get_stride()const { return m_stride; }
    private:


		template<typename E, E v>
		struct collect_element_layout
		{
			static void apply(buffer_element& elements, int& offset, int& stride)
			{
				if (elements.type == v)
				{
					elements.offset = offset;
					elements.count = shader_type_enum_record<v>::count;
					elements.size = sizeof(shader_type_enum_record<v>::type) * elements.count;
					elements.gl_type = shader_type_enum_record<v>::gl_type;

					offset += elements.size;
					stride += elements.size;
				}
			}
		};

		template<typename E, E V>
		static constexpr bool is_valid()
		{
			auto name = __FUNCSIG__;
			int i = 0;
			for (;; i++)
			{
				if (name[i] == '>')
				{
					i--;
					break;
				}
				if (name[i] == '\0')
				{
					return false;
				}

			}


			for (; i >= 0; i--)
			{
				if (name[i] == ')') {
					break;
				}
			}
			if (i == -1)
			{
				return true;
			}

			if (name[i + 1] == '0' && name[i + 2] == 'x')
			{
				return false;
			}

			return true;
		};


		template<typename E, int h = 0 >
		struct loop_enum
		{
			template<template<typename U, U u>  typename F, typename ...P>
			static void apply(P&&... p)
			{
				if constexpr (is_valid<E, E(h)>())
				{
					F<E, E(h)>::apply(std::forward<P>(p)...);
					loop_enum<E, h + 1>::template apply<F>(std::forward<P>(p)...);
				}
			}

		};


        void calculate_offset_and_stride()
		{
			int offset = 0;
			m_stride = 0;
			for (auto& e : m_elements)
			{
				loop_enum<shader_type_enum>::apply<collect_element_layout>(e, offset, m_stride);
			}
		}

        
    private:
        std::vector<buffer_element> m_elements;
        int m_stride;

    };

    class index_buffer
    {
        public:
			index_buffer()
			{
				glCreateBuffers(1, &m_renderer_id);
			}

			~index_buffer()
			{
				glDeleteBuffers(1, &m_renderer_id);
			}

			void set_data(int* vertices, int size)
			{
				m_count = size;
				bind();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(int), vertices, GL_DYNAMIC_DRAW);

			}
			void bind() const
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
			}
			void unbind() const
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

			int get_count() const { return m_count; }
	private:
		uint32_t m_renderer_id;
		uint32_t m_count;

    };


	class vertex_array
	{
	public:
		vertex_array()
		{
			glCreateVertexArrays(1, &m_renderer_id);
		}

		~vertex_array()
		{
			glDeleteVertexArrays(1, &m_renderer_id);
		}

		void bind()
		{
			glBindVertexArray(m_renderer_id);
		}

		void unbind() const
		{
			glBindVertexArray(0);
		}

		void add_vertex_attribute(shader_type_enum data_type, const std::string& name_in_shader)
		{
			if (m_vertex_buffers.count(name_in_shader) && m_vertex_buffers[name_in_shader])
			{
				CE_LOG_ERROR("already have a given name vbo");
				return;
			}

			auto vbo = std::make_unique<vertex_buffer>();
			auto layout = std::make_unique<buffer_layout>();
			layout->add_buffer_elements({ {.name = name_in_shader,.type = data_type} });
			vbo->set_layout(std::move(layout));
			m_vertex_buffers[name_in_shader] = std::move(vbo);
			add_vertex_buffer(name_in_shader);

		}

		void set_vertex_attribute_data(const std::string& name_in_shader, const float* data, int count)
		{

			if (m_vertex_buffers.count(name_in_shader))
			{
				bind();
				auto& vbo = m_vertex_buffers[name_in_shader];
				auto layout = vbo->get_layout().get_elements()[0];
				int size_in_byte = layout.size * count;
				vbo->set_data(data, size_in_byte);
			}
			else
			{
				CE_LOG_ERROR("can't find vertex buffer in vertex array ");
			}
		}

		void set_index_buffer(int *indices, int size)
		{
			bind();
			m_index_buffer = std::make_unique<index_buffer>();
			m_index_buffer->bind();
			m_index_buffer->set_data(indices, size);
		}

		int get_index_count()
		{
			return m_index_buffer->get_count();
		}
		

	private:
		void add_vertex_buffer(const std::string& name_in_shader)
		{
			auto& vb = m_vertex_buffers[name_in_shader];
			if (vb->get_layout().get_elements().empty())
			{
				CE_LOG_ERROR("vertex buffer got no layout yet");
			}

			glBindVertexArray(m_renderer_id);

			vb->bind();

			const auto& layout = vb->get_layout();
			for (const auto& e : layout.get_elements()) //make sure consistent with shader
			{
				glEnableVertexAttribArray(m_location_in_shader);
				glVertexAttribPointer(m_location_in_shader, e.count, e.gl_type, e.normalized, layout.get_stride(), (void*)e.offset);
				m_location_in_shader++;
			}
		}

	private:
		uint32_t m_renderer_id;
		unsigned int m_location_in_shader = 0;
		std::map<std::string, std::unique_ptr<vertex_buffer>> m_vertex_buffers;
		std::unique_ptr<index_buffer> m_index_buffer;
	};

}