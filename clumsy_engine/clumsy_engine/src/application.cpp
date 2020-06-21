
#include <iostream>
#include "application.h"
#include "application_event.h"
#include "log.h"

#include "GLFW/glfw3.h" 


namespace clumsy_engine
{
	Application::Application()
		:m_is_running(true)
	{
		m_window.reset(&Window::create());
		//m_window=std::unique_ptr<Window>(&Window::create());
	}

	Application:: ~Application()
	{
	}


	void Application::run()
	{

		Window_Resize_Event e(100, 200);
		CE_TRACE(e.to_string());
		


		while (m_is_running)
		{
			glClearColor(0.8, 0.1, 0.3, 0.2);
			glClear(GL_COLOR_BUFFER_BIT);

			m_window->on_update();
		}

	}

}
