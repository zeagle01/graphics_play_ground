module;

#include <iostream>

module Main_Window;

import GLFW_Wrapper;


namespace quick_shell
{
	void Main_Window::init()
	{
		m_glfwWrapper = std::make_shared<GLFW_Wrapper>();
	}

	void Main_Window::run_event_loop()
	{
		m_glfwWrapper->main_loop(
			[](int x, int y)
			{
				printf("%d %d \n",x,y);
			}
		);
	}
}
