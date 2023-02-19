

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGL_Wrapper.h"
#include <string> 

namespace soft_render
{

	void OpenGL_Wrapper::init(int width, int height)
	{
		m_width = width;
		m_height = height;

		load_glad();

		create_shader();
		glUseProgram(shader_program);

		make_buffer();

		create_texture();
		bind_texture();
	}


	void OpenGL_Wrapper::load_glad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			printf("glad load failed\n");
		}
		else {

			printf("glad load succend\n");
		}
		printf("gl version %s\n", glGetString(GL_VERSION));

	}

	void OpenGL_Wrapper::bind_texture()
	{
		int active_texture_slot = 0;
		glBindTextureUnit(active_texture_slot, m_texture_id);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);

		glUniform1i(glGetUniformLocation(shader_program, "u_texture"), active_texture_slot);
	}

	void OpenGL_Wrapper::create_shader()
	{
		static std::string vertex_shader_code_as_string = R"(
			#version 330 core
			layout(location=0 ) in vec3 position;
			layout(location=1 ) in vec2 texture;

			out vec3 v_position;
			out vec2 v_texture;

			void main()
			{
				v_position=position;
				v_texture=texture;
				gl_Position=vec4(position,1.0);
			}

		)";

		static std::string fragment_shader_code_as_string = R"(
			#version 330 core
			in vec3 v_position;
			in vec2 v_texture;
			uniform sampler2D u_texture;
			out vec4 color;
			void main()
			{
				color=texture(u_texture,v_texture);
				//color=vec4(v_texture,0.8,1.0);
			}
		)";

		const char* vertex_shader_code = vertex_shader_code_as_string.c_str();
		const char* fragment_shader_code = fragment_shader_code_as_string.c_str();

		GLuint verts_shader;
		verts_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(verts_shader, 1, &vertex_shader_code, NULL);
		glCompileShader(verts_shader);
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(verts_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(verts_shader, 512, NULL, infoLog);
			printf("error vertex shader compilation at vertex shader %s\n", infoLog);
		}
		else {
			printf("vertex shader compiled\n");
		}


		GLuint fragement_shader;
		fragement_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragement_shader, 1, &fragment_shader_code, NULL);
		glCompileShader(fragement_shader);
		glGetShaderiv(fragement_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragement_shader, 512, NULL, infoLog);
			printf("error fragment shader compilation at  %s\n", infoLog);
		}
		else {
			printf("fragment shader compiled\n");
		}


		shader_program = glCreateProgram();
		glAttachShader(shader_program, verts_shader);
		glAttachShader(shader_program, fragement_shader);
		glLinkProgram(shader_program);

		glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader_program, 512, NULL, infoLog);
			printf(" link error %s\n", infoLog);
		}
		else {
			printf(" shader program linking succeed!\n");
		}

		glDeleteShader(verts_shader);
		glDeleteShader(fragement_shader);
	}

	void OpenGL_Wrapper::make_buffer()
	{
		glGenVertexArrays(1, (GLuint*)&m_vao);
		glGenBuffers(1, (GLuint*)&m_vbo);
		glGenBuffers(1, (GLuint*)&m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_positions.size(), m_positions.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_triangles.size(), m_triangles.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void OpenGL_Wrapper::clear()
	{
		glClearColor(0., 0.3, 0.5, 1.);//default color;
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGL_Wrapper::draw(void* data)
	{
		glUseProgram(shader_program);

		glBindTexture(GL_TEXTURE_2D, m_texture_id);
		auto m_data_format = GL_RGBA;
		glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);

		glBindVertexArray(m_vao);

		glDrawElements(GL_TRIANGLES, m_triangles.size(), GL_UNSIGNED_INT, 0);

	}

	void OpenGL_Wrapper::init_texture_data()
	{
		//set data
		int size = m_width * m_height;
		std::vector<uint32_t> white_pixel(size);
		for (int i = 0; i < white_pixel.size(); i++)
		{
			white_pixel[i] = 0xffffffff - i;
		}

		auto m_data_format = GL_RGBA;
		int bpc = m_data_format == GL_RGBA ? 4 : 3;

		glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, white_pixel.data());

	}

	void OpenGL_Wrapper::create_texture()
	{
		auto m_internal_format = GL_RGBA8;

		glCreateTextures(GL_TEXTURE_2D, 1, (GLuint*)&m_texture_id);
		glTextureStorage2D(m_texture_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	}

}
