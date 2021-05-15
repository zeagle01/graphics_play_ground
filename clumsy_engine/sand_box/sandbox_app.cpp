
#include <iostream>
#include <memory>

#include "imgui.h"
#include "clumsy_engine/clumsy_engine.h"
#include "clumsy_engine/entry_point.h"//only included once
#include "sim_app.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "log.h"
#include <algorithm>


class Layer_Demo :public clumsy_engine::Layer
{
public:
	Layer_Demo() :
		clumsy_engine::Layer("demo_layer")
		, m_camara(std::make_shared<clumsy_engine::Orthorgraphic_Camara>())
		, m_camara_NEW(std::make_shared<clumsy_engine::Orthorgraphic_Camara_NEW>())
		, m_dispatcher(std::make_shared < clumsy_engine::Dispatcher<clumsy_engine::Event, bool>>())
		, m_camara_position{ 0, 0, 1.0f }
		, m_camara_up{ 0,1,0 }
		, m_camara_target_position{ 0, 0, 0 }
	{

		m_camara->set_view_field(-1.f, 1.f, -1.f, 1.f);
		m_camara_NEW->set_view_field(-1.f, 1.f, -1.f, 1.f, -0.1f, -100.f);
		m_camara_NEW->set_position(glm::vec3(0.f, 0.f, 3.f));

		/// data
		m_positions = {
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
		m_vertex_array = clumsy_engine::Vertex_Array::create();
		std::shared_ptr<clumsy_engine::Vertex_Buffer > vertex_buffer = clumsy_engine::Vertex_Buffer::create(m_positions.data(), m_positions.size());

		clumsy_engine::Buffer_Layout layout =
		{
			{clumsy_engine::Shader_Data_Type::Float3, "a_position"},
			{clumsy_engine::Shader_Data_Type::Float3, "a_color"}

		};
		vertex_buffer->set_layout(layout);
		m_vertex_array->add_vertex_buffer(vertex_buffer);

		std::shared_ptr<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(triangles.data(), triangles.size());
		m_vertex_array->set_index_buffer(index_buffer);





		std::string vertex_src = R"(
			#version 330 core
			layout(location=0 ) in vec3 position;
			layout(location=1 ) in vec3 a_color;
			out vec3 v_position;
			out vec3 v_color;

			uniform mat4 u_view_projection;
			uniform mat4 u_model_matrix;

			void main()
			{

				//vec3 t_position=u_view_projection*position;
				v_position=position;
				v_color=a_color;
				gl_Position=u_view_projection*u_model_matrix*vec4(position,1.0);
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
		m_shader = std::make_shared<clumsy_engine::Shader>(vertex_src, fragment_src);


		auto key_pressed_handler = [](auto& e)
		{
			return false;
		};

//		m_dispatcher->add<clumsy_engine::Key_Pressed_Event>(
//			std::bind(&Layer_Demo::on_key_pressed, this, std::placeholders::_1)
//			//key_pressed_handler
//			);
	}

	bool on_key_pressed(clumsy_engine::Key_Pressed_Event& e)
	{
//		float speed=0.1f;
//		if (e.get_key() == CE_KEY_LEFT)
//		{
//			m_camara_position.x -= speed;
//		}
//		if (e.get_key() == CE_KEY_RIGHT)
//		{
//			m_camara_position.x += speed;
//		}
//
//		if (e.get_key() == CE_KEY_UP)
//		{
//			m_camara_position.y += speed;
//		}
//		if (e.get_key() == CE_KEY_DOWN)
//		{
//			m_camara_position.y -= speed;
//		}
		return false;

	}

	void on_attach()  override
	{
	};
	void on_detach()override {};
	void on_update(clumsy_engine::Time_Step dt) override
	{

		//CE_INFO("{0} s", dt.get_seconds());

		//dealing move to update way (instead of event)
		float position_speed = 0.1f;
		if (clumsy_engine::Input::is_key_pressed(CE_KEY_LEFT))
		{
			m_camara_position.x -= position_speed * dt;
		}
		if (clumsy_engine::Input::is_key_pressed(CE_KEY_RIGHT))
		{
			m_camara_position.x += position_speed * dt;
		}
		if (clumsy_engine::Input::is_key_pressed(CE_KEY_UP))
		{
			m_camara_position.y += position_speed * dt;
		}
		if (clumsy_engine::Input::is_key_pressed(CE_KEY_DOWN))
		{
			m_camara_position.y -= position_speed * dt;
		}

		float roation_speed = 0.2;
		if (clumsy_engine::Input::is_key_pressed(CE_KEY_A))
		{
			m_camara_rotation += roation_speed * dt;
		}
		if (clumsy_engine::Input::is_key_pressed(CE_KEY_D))
		{
			m_camara_rotation -= roation_speed * dt;
		}

		//mouse handle
		if (m_release) //click
		{
			m_release = false;

			auto newMousePos = clumsy_engine::Input::get_mouse_position();
			m_mouse_pos_x = newMousePos.x;
			m_mouse_pos_y = newMousePos.y;
			CE_CORE_INFO("click");
		}
		else if (!m_release && clumsy_engine::Input::is_mouse_button_pressed(CE_MOUSE_BUTTON_LEFT)) //press
		{
			auto newMousePos = clumsy_engine::Input::get_mouse_position();
			float delta_x = newMousePos.x - m_mouse_pos_x;
			float delta_y = newMousePos.y - m_mouse_pos_y;
			CE_CORE_INFO("mouse x y {0} {1}", delta_x, delta_y);
			m_mouse_pos_x = newMousePos.x;
			m_mouse_pos_y = newMousePos.y;

			m_camara_NEW->dragging_handle(glm::vec2(delta_x, delta_y));
		}
		else//release
		{
			m_release = true;
		}


		clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
		clumsy_engine::Render_Command::clear();



		clumsy_engine::Renderer::begin_scene(m_camara_NEW);

		clumsy_engine::Renderer::submit(m_shader, m_vertex_array, glm::mat4(1.f));

		clumsy_engine::Renderer::end_scene();

	};
	virtual void on_event(clumsy_engine::Event& e) 
	{
		(*m_dispatcher)(e);
	}

	virtual void on_imgui_render(ImGuiContext* imgui_context) 
	{

		ImGui::SetCurrentContext(imgui_context); //cross lib imgui context do not share ! so set it manully

		ImGui::Begin("Test from demo layer");
		ImGui::Text("hello world from demo layer");
		ImGui::End();
	}


private:


	std::shared_ptr<clumsy_engine::Shader> m_shader;
	std::shared_ptr<clumsy_engine::Vertex_Array> m_vertex_array;

	std::shared_ptr<clumsy_engine::Orthorgraphic_Camara> m_camara;
	std::shared_ptr<clumsy_engine::Orthorgraphic_Camara_NEW> m_camara_NEW;


	glm::vec3 m_camara_position ;
	glm::vec3 m_camara_up  ;
	glm::vec3 m_camara_target_position;

	float m_camara_rotation;
	float m_mouse_pos_x = 0.f;
	float m_mouse_pos_y = 0.f;
	float m_theta = 0.f;
	float m_phi = 0.f;
	bool m_release = false;
	std::vector<float> m_positions;

	std::shared_ptr<clumsy_engine::Dispatcher<clumsy_engine::Event, bool>> m_dispatcher;
};


class SanBox_App:public clumsy_engine::Application
{
	public:
		SanBox_App() 
		{

			//std::shared_ptr<clumsy_engine::Layer> layer = std::make_shared<Layer_Demo>();
			push_layer(std::make_shared<Layer_Demo>());
		}

};



std::unique_ptr<clumsy_engine::Application> clumsy_engine::create_application()
{
	clumsy_engine::Log::get_core_logger()->trace("create app");

	std::unique_ptr<clumsy_engine::Application> app = std::make_unique<SanBox_App>(); 
	//std::unique_ptr<clumsy_engine::Application> app = std::make_unique<Sim_App>(); 


//	layer = std::make_shared<clumsy_engine::Imgui_Layer>();
//	app->push_overlay(layer);

	return app;
}


