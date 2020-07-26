
#include <iostream>
#include "application.h"
#include "log.h"
#include <any>
#include <map>
#include "application_event.h"
#include "GLFW/glfw3.h" 


namespace clumsy_engine
{

#define BIND_MEMBER(x) std::bind(&Application::x,this,std::placeholders::_1)






	Application::Application()
		:m_is_running(true)
	{
		m_window = Window::create();
		m_window->set_event_callback(BIND_MEMBER(on_event));

		m_event_fn[Event_Type::Window_Close] = [&](Event& e)
		{
			Window_Close_Event& wc_e = (Window_Close_Event&)e;
			return On_Window_Close(wc_e);
		};
	}

	Application:: ~Application()
	{
		CE_CORE_TRACE("delete application {0}", (void*)this);
	}

	bool Application::on_event(Event& e)
	{

		bool use_dispatcher = true;
		if (use_dispatcher)
		{
			Dispatcher dispatcher(e);
			dispatcher.dispatch<Window_Close_Event>(BIND_MEMBER(On_Window_Close));
		}
		else
		{
			if (m_event_fn.count(e.get_dynamic_type()))
			{
				m_event_fn[e.get_dynamic_type()](e);
			}
		}


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


	bool Application::On_Window_Close(Window_Close_Event& e)
	{
		m_is_running = false;
		return true;
	}

}
