
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
	}

	virtual void on_attach() 
	{
	};
	virtual void on_detach() {};
	virtual void on_update()
	{
		clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
		clumsy_engine::Render_Command::clear();

		m_camara->set_position({ 0.5, 0.5, 0 });
		m_camara->set_rotation(45.f);

		clumsy_engine::Renderer::begin_scene(m_camara);

		clumsy_engine::Renderer::submit(m_shader, m_vertex_array);

		clumsy_engine::Renderer::end_scene();

	};
	virtual void on_event(clumsy_engine::Event& e) 
	{
		//CE_TRACE("layer {0} handle {1} ", (void*)this, e.to_string());
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


