
#pragma once

#include <memory>
#include <map>
#include <functional>
#include "window.h"



namespace clumsy_engine
{

	class Event;
	class Window_Close_Event;


	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:
		bool on_event(Event& e);

		std::unique_ptr<Window> m_window;

		std::map<Event_Type,std::function<bool (Event& e)>> m_event_fn;

		bool m_is_running;


	private:
		bool On_Window_Close(Window_Close_Event& e);
	};

	extern std::unique_ptr<clumsy_engine::Application> create_application();
}
