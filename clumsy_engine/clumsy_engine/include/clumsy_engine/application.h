
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
		std::unique_ptr<Window> m_window;
		bool m_is_running;
	};

	Application* create_application();
}
