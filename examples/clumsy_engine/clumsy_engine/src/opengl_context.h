
#pragma once

#include "graphic_context.h"

struct GLFWwindow;

namespace clumsy_engine
{



	class OpenGL_Context:public Graphic_Context
	{

	public:
		OpenGL_Context(GLFWwindow* window);

		void init() override;

		void swap_buffers() override;


	private:
		GLFWwindow* m_window;
	};

}
