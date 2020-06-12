
#include <iostream>
#include "application.h"
#include "application_event.h"
#include "log.h"


namespace clumsy_engine
{
	Application::Application()
	{
	}

	Application:: ~Application()
	{
	}


	void Application::run()
	{

		Window_Resize_Event e(100, 200);
		CE_TRACE(e.to_string());
		


		while (true)
		{

		}

	}

}
