
#pragma once

#include <memory>
#include "window.h"



namespace clumsy_engine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:
		bool on_event(Event& e);

		std::unique_ptr<Window> m_window;
		bool m_is_running;
	};

	extern std::unique_ptr<clumsy_engine::Application> create_application();
}
