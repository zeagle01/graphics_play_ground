#ifndef SHADER_H
#define SHADER_H


#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Shader
{

private:
	GLuint shader_program;
	const char* vertex_shader_code;
	const char* fragment_shader_code;
public:
	Shader();
	virtual ~Shader();
	Shader(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
	void use();
	GLint get_uniform_variable(const std::string& variable_name);
	GLint get_program();

	void set_int(std::string,int v);
	void set_float(std::string,float  v);
	void set_bool(std::string,bool v);
};



#endif
