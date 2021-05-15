
#include <iostream>
#include <memory>
#include <chrono>
#include "application.h"
#include "log.h"
#include "application_event.h"
#include "layer_stack.h"
#include "layer.h"
#include "window.h"
#include "imgui_layer.h"
#include "input.h"
#include "dispatcher.h"
#include "shader.h"
#include "buffer.h"
#include "vertex_array.h"
#include "renderer.h"
#include "render_command.h"
#include "camara.h"
#include "time_step.h"


namespace clumsy_engine
{

#define BIND_MEMBER(x) std::bind(&Application::x,this,std::placeholders::_1)



	static std::unique_ptr<Application> s_singleton;
	Application& Application::get_singleton()
	{
		if (!s_singleton)
		{
			s_singleton = std::make_unique<Application>();
		}
		return *s_singleton;
	}


	Application::Application()
		:m_is_running(true)
		, m_layer_stack(std::make_unique<Layer_Stack>())
		, m_dispatcher_imp(std::make_shared<Dispatcher<Event, bool>>())
		, m_dispatcher(*m_dispatcher_imp)
	{
		m_window = Window::create();
		m_window->set_event_callback(BIND_MEMBER(on_event));
		m_window->set_vertical_sync(false);


		m_dispatcher_imp->add<Window_Close_Event>(BIND_MEMBER(On_Window_Close));


		s_singleton.reset(this);

		m_imgui_layer = std::make_shared<Imgui_Layer>();
		push_overlay(m_imgui_layer);

	}

	Application:: ~Application()
	{
		CE_CORE_TRACE("delete application {0}", (void*)this);
	}

	bool Application::on_event(Event& e)
	{

		m_dispatcher(e);


		for (auto it = m_layer_stack->end(); it != m_layer_stack->begin();)
		{
			(*(--it))->on_event(e);
			
			if (e.is_handled())
			{
				break;
			}
		}


		//CE_TRACE(e.to_string());

		return true;
	}

	void Application::run()
	{

//		Window_Resize_Event e(100, 200);
//		CE_TRACE(e.to_string());
		


		while (m_is_running)
		{

			float time = std::chrono::high_resolution_clock::now().time_since_epoch().count()*1e-9;

			Time_Step time_step = time - m_last_frame_time;

			m_last_frame_time = time;

			for (auto& layer : *m_layer_stack)
			{
				layer->on_update(time_step);
			}

			m_imgui_layer->begin();

			for (auto& layer : *m_layer_stack)
			{
				layer->on_imgui_render(m_imgui_layer->get_imgui_context());
			}



			m_imgui_layer->end();

//			auto [x, y] = Input::get_mouse_position();
//
//			CE_CORE_TRACE("{0},{1}", x, y);

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
