
#include "shader.h"
#include "glog/logging.h"
#include "glad/glad.h"
#include "renderer.h"

#include <string>
#include <fstream>
#include <sstream>

Shader::Shader():m_id(0)
{
}

Shader::~Shader()
{
    GL_Call(glDeleteProgram(m_id));
}

std::string Shader::read_source_from_file(const std::string& file)
{
	std::ifstream fin(file);
	if (!fin.good())
	{
		LOG(ERROR) << "faile to open file";
	}

	std::stringstream ss;
	ss << fin.rdbuf();
	return ss.str();
}

unsigned int Shader::compile_shader(std::string& shader,unsigned int type)
{

	unsigned int id = glCreateShader(type);
	const char* src = shader.data();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int lengh;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lengh);
		std::string log_s(lengh,' ');
		glGetShaderInfoLog(id, lengh, &lengh, &log_s[0]);
		auto shader_name=(type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
		LOG(ERROR) << "faile to compile " << shader_name;
		LOG(ERROR) << log_s;
	}

	return id;

}

void Shader::create_shader_from_file(const std::string &vertex_shader,const  std::string &fragment_shader)
{
    create_shader(read_source_from_file(vertex_shader), read_source_from_file(fragment_shader));
}

void Shader::create_shader(std::string& vertex_shader, std::string& fragment_shader)
{
	auto program = glCreateProgram();
	auto vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	auto fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

    m_id = program;

}

void Shader::bind() const
{
    GL_Call(glUseProgram(m_id));
}
void Shader::unbind() const
{
    GL_Call(glUseProgram(0));
}

int Shader::get_uniform_location(const std::string &name)
{

	int location = -1;
	if (m_uniform_location_cache.count(name))
	{
		location = m_uniform_location_cache[name];
	}
	else
	{
		GL_Call(auto location = glGetUniformLocation(m_id, name.data()));
		m_uniform_location_cache[name] = location;
	}

    if (location == -1)
    {
        LOG(WARNING) << "can't find uniform variable " << name;
    }
	return location;
}

void Shader::set_uniform_4f(const std::string &name, float v0, float v1, float v2, float v3)
{
	auto location = get_uniform_location(name);
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::set_uniform_mat4f(const std::string &name, float* mat4)
{
	auto location = get_uniform_location(name);
	GL_Call(glUniformMatrix4fv(location, 1, GL_FALSE, mat4));
}

void Shader::set_uniform_1i(const std::string& name, int v)
{
	auto location = get_uniform_location(name);
    glUniform1i(location, v);
}

void Shader::set_uniform_1iv(const std::string &name,const unsigned int count,const int* v)
{
	auto location = get_uniform_location(name);
	glUniform1iv(location, count, v);
}
