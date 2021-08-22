
#pragma once

#include "ref.h"

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
	class Imgui_Layer;
	class Vertex_Buffer;
	class Index_Buffer;
	class Vertex_Array;
	enum class Event_Type;
	template<typename  Base_T, typename Ret  >
	class Dispatcher;
	class Orthorgraphic_Camara;


	class Application
	{
	public:
		static Application& get_singleton();

		Application();

		virtual ~Application();

		void run();

		void push_layer(Ref<Layer> layer);
		void push_overlay(Ref<Layer> overlay);

		Window& get_window() const;



	private:
		void application_update();

		bool on_event(Event& e);

		std::unique_ptr<Window> m_window;
		Ref<Imgui_Layer> m_imgui_layer;

		std::map<Event_Type,std::function<bool (Event& e)>> m_event_fn;

		Ref<Dispatcher<Event,  bool>> m_dispatcher_imp;
		Dispatcher<Event, bool>& m_dispatcher;

		std::unique_ptr<Layer_Stack> m_layer_stack;

		float m_last_frame_time;

		bool m_is_running;

	private:
		bool On_Window_Close(Window_Close_Event& e);

	};

	extern std::unique_ptr<clumsy_engine::Application> create_application();

	bool run_test(int argc,char** argv);
}
