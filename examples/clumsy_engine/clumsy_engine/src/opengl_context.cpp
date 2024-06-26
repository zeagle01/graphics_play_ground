

#include "opengl_context.h"


#include "helper_utils.h"
#include "log.h"

#include "glad/glad.h" //put glad first before other opengl include
#include "GLFW/glfw3.h"



namespace clumsy_engine
{

	OpenGL_Context::OpenGL_Context(GLFWwindow* window) 
		:m_window(window)
	{
		CE_CORE_ASSERT(window, "window is nullptr");

	}

	void OpenGL_Context::init() 
	{

		glfwMakeContextCurrent(m_window);

		//right after make context current(need a context)
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CE_CORE_ASSERT(status, "glad load failed");
		CE_CORE_INFO("gl rednerer: {0}", (const char*)glGetString(GL_RENDERER));
		CE_CORE_INFO("gl verdor: {0}", (const char*)glGetString(GL_VENDOR));
		CE_CORE_INFO("gl version: {0}", (const char*)glGetString(GL_VERSION));

	}


	void OpenGL_Context::swap_buffers()
	{
		glfwSwapBuffers(m_window);
	}

}

