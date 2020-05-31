
#pragma once

namespace clumsy_engine
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

	};

	Application* create_application();
}
