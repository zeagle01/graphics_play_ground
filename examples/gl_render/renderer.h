
#pragma once

#include <string>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"



#define ASSERT(x) if(!(x)) __debugbreak();
#define GL_Call(function)\
	gl_clear_error();\
	function;\
	ASSERT(gl_check_error(#function,__FILE__,__LINE__))\

void gl_clear_error();
bool gl_check_error(const std::string & function, const std::string & file, int line);







class Renderer
{
	private:

	public:
	void draw(const Vertex_Array& va,const Index_Buffer& ib,const Shader& shader);

	void clear();
};