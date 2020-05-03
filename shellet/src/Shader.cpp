
#include <fstream>
#include <sstream>

#include "Shader.h"


#include "glog/logging.h"

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

Shader::Shader(const std::string& vertex_shader_file, const std::string& fragment_shader_file) {
    std::ifstream v_s_fin;
    std::ifstream f_s_fin;
    v_s_fin.open(vertex_shader_file);
    if (!v_s_fin) {
		LOG(ERROR)<<"fail to open vertex source file: "<<vertex_shader_file ;
	}
	else {
		LOG(INFO)<<"open vertex source file: "<<vertex_shader_file ;
	}

    f_s_fin.open(fragment_shader_file);
    if (!f_s_fin) {
		LOG(ERROR)<<"fail to open fragment source file" ;
	}
	else {
		LOG(INFO)<<"open fragment source file: " <<fragment_shader_file;
	}
    std::stringstream v_s_stream, f_s_stream;
    v_s_stream << v_s_fin.rdbuf();
    f_s_stream << f_s_fin.rdbuf();


    std::string vertex_shader_code_as_string(v_s_stream.str());
    vertex_shader_code = vertex_shader_code_as_string.c_str();
    std::string fragment_shader_code_as_string(f_s_stream.str());
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
		LOG(ERROR)<<"error vertex shader compilation at " <<vertex_shader_file;
		LOG(ERROR)<<infoLog;
	}
	else {
		LOG(INFO)<<"vertex shader compilation:" <<vertex_shader_file;
	}


    GLuint fragement_shader;
    fragement_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragement_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragement_shader);
    glGetShaderiv(fragement_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
		glGetShaderInfoLog(fragement_shader, 512, NULL, infoLog);
		LOG(ERROR)<<"error fragment shader compilation at " <<fragment_shader_file;
		LOG(ERROR)<<infoLog;
	}
	else {
		LOG(INFO) << "fragment shader compilation: " << fragment_shader_file;
	}


    shader_program = glCreateProgram();
    glAttachShader(shader_program, verts_shader);
    glAttachShader(shader_program, fragement_shader);
    glLinkProgram(shader_program);

    glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
		glGetShaderInfoLog(shader_program, 512, NULL, infoLog);
		LOG(ERROR)<<"error shader program linking" ;
		LOG(ERROR)<<infoLog;
	}
	else {
		LOG(INFO)<<" shader program linking succeed!" ;
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
        LOG(ERROR)<<"can't find uniform!";
    }
    return location;
}

GLint Shader::get_program() {
    return shader_program;
}
