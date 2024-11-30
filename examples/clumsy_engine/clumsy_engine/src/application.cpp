
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
#include "renderer_2D.h"
#include "render_command.h"
#include "camara.h"
#include "time_step.h"
#include "profiler.h"

#include <memory>
#include <iostream>
#include <memory>
#include <chrono>


namespace clumsy_engine
{

#define BIND_MEMBER(x) std::bind(&Application::x,this,std::placeholders::_1)



	static Application* s_singleton; //life time controled by user
	Application& Application::get_singleton()
	{
		return *s_singleton;
	}


	Application::Application()
		:m_is_running(true)
		, m_layer_stack(std::make_unique<Layer_Stack>())
		, m_dispatcher_imp(std::make_shared<Dispatcher<Event, bool>>())
		, m_dispatcher(*m_dispatcher_imp)
	{

		s_singleton = this;

		m_window = Window::create();
		m_window->set_event_callback(BIND_MEMBER(on_event));
		m_window->set_vertical_sync(false);

		Renderer::init();
		Renderer_2D::init();

		m_dispatcher_imp->add<Window_Close_Event>(BIND_MEMBER(On_Window_Close));
		m_dispatcher_imp->add<Window_Resize_Event>(BIND_MEMBER(On_Window_Resized));

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


	void Application::application_update()
	{
		RECORD_FUNCTION_DURATION();

		float time = std::chrono::high_resolution_clock::now().time_since_epoch().count() * 1e-9;

		Time_Step time_step = time - m_last_frame_time;

		m_last_frame_time = time;

		if (!m_is_minimized)
		{
			for (auto& layer : *m_layer_stack)
			{
				layer->on_update(time_step);
			}
		}

		if (m_imgui_layer)
		{
			m_imgui_layer->begin();

			for (auto& layer : *m_layer_stack)
			{
				layer->on_imgui_render(m_imgui_layer->get_imgui_context());
			}
			m_imgui_layer->end();
		}

		m_window->on_update();
	}

	void Application::run()
	{

		while (m_is_running)
		{
			application_update();
		}

	}

	void Application::close()
	{
		m_is_running = false;
	}


	bool Application::On_Window_Close(Window_Close_Event& e)
	{
		
		m_is_running = false;
		return true;
	}

	bool Application::On_Window_Resized(Window_Resize_Event& e)
	{
		if (e.get_width() == 0 || e.get_height() == 0)
		{
			m_is_minimized = true;
		}
		else
		{
			m_is_minimized = false;
		}

		Renderer::on_window_resized(e.get_width(), e.get_height());

		return false;
	}

	void Application::push_layer(Ref<Layer> layer)
	{
		m_layer_stack->push_layer(layer);
		layer->on_attach();
	}

	void Application::push_overlay(Ref<Layer> overlay)
	{
		m_layer_stack->push_overlay(overlay);
		overlay->on_attach();
	}

	Window& Application::get_window()const
	{
		return *m_window;
	}

}
