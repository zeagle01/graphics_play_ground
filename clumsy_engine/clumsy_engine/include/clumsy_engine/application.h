
#pragma once

namespace clumsy_engine
{
	void print();

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

	};

	Application* create_application();
}
