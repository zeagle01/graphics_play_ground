
#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>

class Shader
{

private:
	GLuint shader_program;
	const char* vertex_shader_code;
	const char* fragment_shader_code;
public:
	Shader();
	virtual ~Shader();
	void init();
	void use();
	GLint get_uniform_variable(const std::string& variable_name);
	GLint get_program();

	void set_int(std::string,int v);
	void set_float(std::string,float  v);
	void set_bool(std::string,bool v);
};
