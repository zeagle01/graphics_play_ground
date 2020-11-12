
#include <iostream>
#include <memory>
#include "application.h"
#include "log.h"
#include "application_event.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h" 
#include "layer_stack.h"
#include "layer.h"
#include "window.h"
#include "imgui_layer.h"
#include "input.h"
#include "dispatcher.h"

#include "gmock/gmock.h"


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
		,m_dispatcher_imp(std::make_shared<Dispatcher<Event, bool>>())
		,m_dispatcher(*m_dispatcher_imp)
	{
		m_window = Window::create();
		m_window->set_event_callback(BIND_MEMBER(on_event));


		m_dispatcher_imp->add<Window_Close_Event>(BIND_MEMBER(On_Window_Close));


		s_singleton.reset(this);

		m_imgui_layer = std::make_shared<Imgui_Layer>();
		push_overlay(m_imgui_layer);


		//gl data stuff
		glGenVertexArrays(1,&m_vertex_array);
		glBindVertexArray(m_vertex_array);

		glGenBuffers(1, &m_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

		std::vector<float> positions{
			0.,0,0,
			1,0,0,
			1,1,0
		};

		std::vector<int> triangles{
			0,1,2
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * triangles.size(), triangles.data(), GL_DYNAMIC_DRAW);
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
			glClearColor(0.2, 0.1, 0.3, 0.2);
			glClear(GL_COLOR_BUFFER_BIT);

			std::vector<float> pp{
			0.,0,0,
			-1,0,0,
			1,1,0
			};

			glBindVertexArray(m_vertex_array);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pp.size(), pp.data(), GL_DYNAMIC_DRAW);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (auto& layer : *m_layer_stack)
			{
				layer->on_update();
			}

			m_imgui_layer->begin();

			for (auto& layer : *m_layer_stack)
			{
				//m_imgui_layer->begin();
				layer->on_imgui_render();
				//m_imgui_layer->end();
			}

			//m_imgui_layer->on_imgui_render();

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

	bool run_test(int argc,char** argv)
	{

		::testing::InitGoogleMock(&argc, argv);


		auto failed = RUN_ALL_TESTS();
		//if (failed)
		//{
		//	std::getchar();
		//}

		std::getchar();

		return true;
	}

}
