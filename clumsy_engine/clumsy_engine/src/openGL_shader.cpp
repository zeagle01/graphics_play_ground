
#include <string>
#include <vector>
#include <array>

#include "openGL_shader.h"
#include "glad/glad.h"

#include "log.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>

namespace clumsy_engine
{

    static unsigned int get_shader_type_from_string(const std::string& shader_type)
    {

        if (shader_type == "vertex")
        {
            return GL_VERTEX_SHADER;

        }
        else if (shader_type == "fragment")
        {
            return GL_FRAGMENT_SHADER;
        }
        else
        {
            CE_ERROR("not support shader type {0}", shader_type);
            return 0;
        }
    }

	std::string OpenGL_Shader::read_shader_file(const std::string& shader_file)
	{
        std::string source_code;
		std::ifstream fin(shader_file, std::ios::in | std::ios::binary);
        if (fin)
        {
            fin.seekg(0, std::ios::end);
            source_code.resize(fin.tellg());
            fin.seekg(0, std::ios::beg);
            fin.read(&source_code[0], source_code.size());
            fin.close();
        }
        else
        {
			CE_ERROR("can't open shader file {0}", shader_file);
        }
        return source_code;
	}

	std::unordered_map<unsigned int, std::string> OpenGL_Shader::split_according_to_shader_type(const std::string& all_in_one_source_code)
	{
        std::unordered_map<unsigned int, std::string> shader_sources_by_type;

        std::string type_token = "#shader_type";
        size_t type_token_pos = all_in_one_source_code.find(type_token, 0);
		while (type_token_pos != std::string::npos)
        {
            size_t end_of_line = all_in_one_source_code.find_first_of("\r\n", type_token_pos);
            size_t begin = type_token_pos + type_token.size() + 1;
            std::string type = all_in_one_source_code.substr(begin, end_of_line - begin);

            assert(type == "vertex" || type == "fragment" || type == "pixel");

            size_t next_line_begin = all_in_one_source_code.find_first_not_of("\r\n", end_of_line);
            size_t next_type_token_pos = all_in_one_source_code.find(type_token, next_line_begin); //find next type token
			size_t shader_source_block_size = next_type_token_pos - next_line_begin;
			shader_sources_by_type[get_shader_type_from_string(type)] = all_in_one_source_code.substr(next_line_begin, shader_source_block_size);

            type_token_pos = next_type_token_pos;
        }
        return shader_sources_by_type;
	}

	void OpenGL_Shader::compile(const std::unordered_map<unsigned int, std::string>& shader_sources_by_type)
	{
		GLuint program = glCreateProgram();

        constexpr unsigned int max_shader_type_size = 5;
		std::array<unsigned int, max_shader_type_size> shader_id;
		size_t shader_id_size = 0;

		for (const auto& shader_it : shader_sources_by_type)
		{
			auto shader_type = shader_it.first;
            const auto& shader_source = shader_it.second;


			GLuint shader = glCreateShader(shader_type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source_ctr = (const GLchar*)shader_source.c_str();
			glShaderSource(shader, 1, &source_ctr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				CE_CORE_ERROR("vertex compile fail: {0}", infoLog.data());

				// In this simple program, we'll just leave
				break;
			}
            shader_id[shader_id_size++] = shader;
			glAttachShader(program, shader);

		}

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            // Don't leak shaders either.
            for (int i = 0; i < shader_id_size; i++)
            {
                glDeleteShader(shader_id[i]);
            }

            // Use the infoLog as you see fit.
            CE_CORE_ERROR("linking fail: {0}", infoLog.data());

            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
		for (int i = 0; i < shader_id_size; i++)
		{
			glDetachShader(program, shader_id[i]);
		}

        m_renderer_id = program;
	}

	OpenGL_Shader::OpenGL_Shader(const std::string& shader_file)
	{
		std::string source_code = read_shader_file(shader_file);
		auto shader_sources_by_type = split_according_to_shader_type(source_code);
        compile(shader_sources_by_type);
	}

    OpenGL_Shader::OpenGL_Shader(const std::string &vertex_src, std::string &fragment_src)
    {
		std::unordered_map<unsigned int, std::string> shader_sources_by_type;
        shader_sources_by_type[GL_VERTEX_SHADER] = vertex_src;
        shader_sources_by_type[GL_FRAGMENT_SHADER] = fragment_src;
        compile(shader_sources_by_type);
    }

    void OpenGL_Shader::bind() const
    {
        glUseProgram(m_renderer_id);

    }

    void OpenGL_Shader::unbind() const
    {
        glUseProgram(0);
    }

	void OpenGL_Shader::upload_uniform_mat4(const std::string& name,const glm::mat4& m)
	{
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
	}

	void OpenGL_Shader::upload_uniform_vec3(const std::string& name, const glm::vec3& m)
	{
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(m));
	}

    void OpenGL_Shader::upload_uniform_float(const std::string& name, const float& v)
	{
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1fv(location, 1, &v);
	}

	void OpenGL_Shader::upload_uniform_int(const std::string& name, const int& v)
	{
		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1iv(location, 1, &v);
	}

}  
