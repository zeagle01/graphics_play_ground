
#include <string>
#include <vector>

#include "openGL_shader.h"
#include "glad/glad.h"

#include "log.h"
#include "glm/gtc/type_ptr.hpp"

namespace clumsy_engine
{
    OpenGL_Shader::OpenGL_Shader(const std::string &vertex_src, std::string &fragment_src)
    {
        // Read our shaders into the appropriate buffers
        std::string vertexSource = vertex_src;    // Get source code for vertex shader.
        std::string fragmentSource = fragment_src; // Get source code for fragment shader.

        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar *source = (const GLchar *)vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.
            CE_CORE_ERROR("vertex compile fail: {0}", infoLog.data());

            // In this simple program, we'll just leave
            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = (const GLchar *)fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.
            CE_CORE_ERROR("fragment compile fail: {0}", infoLog.data());

            // In this simple program, we'll just leave
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        GLuint program = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

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
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Use the infoLog as you see fit.
            CE_CORE_ERROR("linking fail: {0}", infoLog.data());

            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);


        //store as my renderer id
        m_renderer_id = program;
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
