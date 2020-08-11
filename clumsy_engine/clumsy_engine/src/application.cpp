
#include <iostream>
#include <memory>
#include "application.h"
#include "log.h"
#include "application_event.h"
#include "GLFW/glfw3.h" 
#include "layer_stack.h"
#include "layer.h"
#include "window.h"
#include "imgui_layer.h"


namespace clumsy_engine
{

#define BIND_MEMBER(x) std::bind(&Application::x,this,std::placeholders::_1)




	Application& Application::get_singleton()
	{
		static std::unique_ptr<Application> s=std::make_unique<Application>();
		return *s;
	}


	Application::Application()
		:m_is_running(true)
		,m_layer_stack(std::make_unique<Layer_Stack>())
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

		for (auto it = m_layer_stack->end(); it != m_layer_stack->begin();)
		{
			(*(--it))->on_event(e);
			
			if (e.is_handled())
			{
				break;
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

			for (auto& layer : *m_layer_stack)
			{
				layer->on_update();
			}

			m_window->on_update();
		}

	}


	bool Application::On_Window_Close(Window_Close_Event& e)
	{
		m_is_running = false;
		return true;
	}

	void Application::push_layer(std::shared_ptr<Layer> layer)
	{
		m_layer_stack->push_layer(layer);
		layer->on_attach();
	}

	void Application::push_overlay(std::shared_ptr<Layer> overlay)
	{
		m_layer_stack->push_overlay(overlay);
		overlay->on_attach();
	}

	Window& Application::get_window()const
	{
		return *m_window;
	}


}
