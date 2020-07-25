
#include <iostream>
#include "application.h"
#include "application_event.h"
#include "log.h"

#include "GLFW/glfw3.h" 


namespace clumsy_engine
{

#define BIND_MEMBER(x) std::bind(&Application::x,this,std::placeholders::_1)



	Application::Application()
		:m_is_running(true)
	{
		m_window = Window::create();
		m_window->set_event_callback(BIND_MEMBER(on_event));
	}

	Application:: ~Application()
	{
		CE_CORE_TRACE("delete application {0}", (void*)this);
	}

	bool Application::on_event(Event& e)
	{


		Dispatcher dispatcher(e);

		dispatcher.dispatch<Window_Close_Event>([&](Window_Close_Event& e) {
			m_is_running = false;
			return true;
		});

		CE_TRACE(e.to_string());

		return true;
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
