

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
