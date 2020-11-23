
#include <iostream>
#include <memory>

#include "imgui.h"
#include "clumsy_engine/clumsy_engine.h"


class Layer_Demo :public clumsy_engine::Layer
{
public:
	Layer_Demo() :
		clumsy_engine::Layer("demo_layer")
		, m_camara(std::make_shared<clumsy_engine::Orthorgraphic_Camara>(-1,1,-1,1))
		, m_dispatcher(std::make_shared < clumsy_engine::Dispatcher<clumsy_engine::Event, bool>>())
		, m_camara_position(0.f)
		,m_camara_rotation(0.f)
	{

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
		m_vertex_array = clumsy_engine::Vertex_Array::create();
		std::shared_ptr<clumsy_engine::Vertex_Buffer > vertex_buffer = clumsy_engine::Vertex_Buffer::create(positions.data(), positions.size());

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

			void main()
			{

				//vec3 t_position=u_view_projection*position;
				v_position=position;
				v_color=a_color;
				gl_Position=u_view_projection*vec4(position,1.0);
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

		CE_INFO("{0} s", dt.get_seconds());
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

		clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
		clumsy_engine::Render_Command::clear();

		m_camara->set_position(m_camara_position);
		m_camara->set_rotation(m_camara_rotation);

		clumsy_engine::Renderer::begin_scene(m_camara);

		clumsy_engine::Renderer::submit(m_shader, m_vertex_array);

		clumsy_engine::Renderer::end_scene();

	};
	virtual void on_event(clumsy_engine::Event& e) 
	{
		(*m_dispatcher)(e);
	}

	virtual void on_imgui_render() 
	{

		////do not work for imgui context reason
//		ImGui::Begin("Test");
//		ImGui::Text("hello world");
//		ImGui::End();
	}

private:

	std::shared_ptr<clumsy_engine::Shader> m_shader;
	std::shared_ptr<clumsy_engine::Vertex_Array> m_vertex_array;

	std::shared_ptr<clumsy_engine::Orthorgraphic_Camara> m_camara;

	glm::vec3 m_camara_position;

	float m_camara_rotation;

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


//	layer = std::make_shared<clumsy_engine::Imgui_Layer>();
//	app->push_overlay(layer);

	return app;
}


