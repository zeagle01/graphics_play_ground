
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
#include "shader.h"
#include "buffer.h"
#include "vertex_array.h"

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

		/// data
		std::vector<float> positions{
			0.,0,0,0,0,0.5,
			1,0,0,0,0,0.2,
			1,1,0,0,0,0.2
		};
//		std::vector<float> positions{
//			0.,0,0,
//			1,0,0,
//			1,1,0,
//		};

		std::vector<int> triangles{
			0,1,2
		};

		//gl data stuff
		m_vertex_array = Vertex_Array::create();
		std::shared_ptr<Vertex_Buffer > vertex_buffer = Vertex_Buffer::create(positions.data(), positions.size());

		Buffer_Layout layout =
		{
			{Shader_Data_Type::Float3, "a_position"},
			{Shader_Data_Type::Float3, "a_color"}

		};
		vertex_buffer->set_layout(layout);
		m_vertex_array->add_vertex_buffer(vertex_buffer);

		std::shared_ptr<Index_Buffer> index_buffer = Index_Buffer::create(triangles.data(), triangles.size());
		m_vertex_array->set_index_buffer(index_buffer);





		std::string vertex_src = R"(
			#version 330 core
			layout(location=0 ) in vec3 position;
			layout(location=1 ) in vec3 a_color;
			out vec3 v_position;
			out vec3 v_color;

			void main()
			{
				v_position=position;
				v_color=a_color;
				gl_Position=vec4(position,1.0);
			}

		)";

		std::string fragment_src = R"(

			#version 330 core
			in vec3 v_position;
			in vec3 v_color;
			out vec4 color;

			void main()
			{
				color=vec4(v_position+v_color,1.0);
				//color=vec4(v_position,1.0);
			}

		)";

		//shader
		m_shader = std::make_unique<Shader>(vertex_src, fragment_src);
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

//			std::vector<float> pp{
//			0.,0,0,
//			-1,0,0,
//			1,1,0
//			};

			m_shader->bind();

			m_vertex_array->bind();
			//m_vertex_buffer->bind();

//			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pp.size(), pp.data(), GL_DYNAMIC_DRAW);

			glDrawElements(GL_TRIANGLES, m_vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);

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
