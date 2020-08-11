
#pragma once

#include <memory>
#include <map>
#include <functional>



namespace clumsy_engine
{

	class Event;
	class Window_Close_Event;
	class Layer_Stack;
	class Layer;
	class Window;
	enum class Event_Type;


	class Application
	{
	public:
		static Application& get_singleton();

		Application();

		virtual ~Application();

		void run();

		void push_layer(std::shared_ptr<Layer> layer);
		void push_overlay(std::shared_ptr<Layer> overlay);

		Window& get_window() const;



	private:

		bool on_event(Event& e);

		std::unique_ptr<Window> m_window;

		std::map<Event_Type,std::function<bool (Event& e)>> m_event_fn;

		bool m_is_running;

		std::unique_ptr<Layer_Stack> m_layer_stack;

	private:
		bool On_Window_Close(Window_Close_Event& e);
	};

	extern std::unique_ptr<clumsy_engine::Application> create_application();
}
