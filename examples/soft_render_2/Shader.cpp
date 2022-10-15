

#include <fstream>
#include <sstream>
#include <string>

#include "Shader.h"


Shader::Shader() {
}


Shader::~Shader() {
}


void Shader::set_int(std::string variable,int value){
    glUniform1i(glGetUniformLocation(shader_program,variable.c_str()),value);
}
void Shader::set_float(std::string variable,float value){
    glUniform1f(glGetUniformLocation(shader_program,variable.c_str()),value);
}
void Shader::set_bool(std::string variable,bool value){
    glUniform1i(glGetUniformLocation(shader_program,variable.c_str()),(int)value);
}

void Shader::init()
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

	vertex_shader_code = vertex_shader_code_as_string.c_str();
	fragment_shader_code = fragment_shader_code_as_string.c_str();

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
		printf(" shader program linking succeed!");
	}

	glDeleteShader(verts_shader);
	glDeleteShader(fragement_shader);

}


void Shader::use() {
    glUseProgram(shader_program);
}

GLint Shader::get_uniform_variable(const std::string & variable_name)
{
    GLint location= glGetUniformLocation(shader_program, variable_name.c_str());
    if(location==GL_INVALID_INDEX)
    {
        //LOG(ERROR)<<"can't find uniform!";
    }
    return location;
}

GLint Shader::get_program() {
    return shader_program;
}
