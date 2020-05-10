

#include "renderer.h"

#include "glog/logging.h"
#include "glad/glad.h" 

void gl_clear_error()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}

}
bool gl_check_error(const std::string & function, const std::string & file, int line)
{

	while (auto error = glGetError())
	{

		LOG(ERROR) << "file: " <<file <<" function: "<<function<<" line: " <<line;
		LOG(ERROR) << "[OpenGL Error]: " << error;
		return false;
	}
	return true;
}

void Renderer::draw(const Vertex_Array &va, const Index_Buffer &ib, const Shader &shader)
{
	shader.bind();
	va.bind();
	ib.bind();

	GL_Call(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}


void Renderer::clear()
{
	GL_Call(glClear(GL_COLOR_BUFFER_BIT));
}