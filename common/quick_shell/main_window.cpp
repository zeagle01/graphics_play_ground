module;

#include <iostream>

module main_window;



namespace quick_shell
{
	void main_window::init()
	{
		m_glfwWrapper = std::make_shared<GLFW_wrapper>();
		m_glfwWrapper->create_window(800, 600);
	}

	void main_window::run_event_loop()
	{
		m_glfwWrapper->main_loop(
			[](int x, int y)
			{
				printf("%d %d \n",x,y);
			}
		);
	}
}
