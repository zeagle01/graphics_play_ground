
#include <iostream>
#include <memory>

#include "imgui.h"
#include "clumsy_engine/clumsy_engine.h"
#include "clumsy_engine/entry_point.h"//only included once
#include "sim_app.h"
#include "profiler.h"
#include "clumsy_engine/openGL_shader.h"
#include "profiler.h"
#include "clumsy_engine/ref.h"
#include "clumsy_engine/file_dialogs.h"

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
		, m_dispatcher(std::make_shared < clumsy_engine::Dispatcher<clumsy_engine::Event, bool>>())
	{

		m_camara->set_view_field(-1.f, 1.f, -1.f, 1.f, -0.1f, -100.f);
		m_camara->look_at(
			glm::vec3(0.f,0.f,3.f),
			glm::vec3(0.f,0.f,0.f),
			glm::vec3(0.f,1.f,0.f)
		);

		/// data
		config_shader();
		config_shader_plane();

		auto key_pressed_handler = [](auto& e)
		{
			return false;
		};

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
		RECORD_FUNCTION_DURATION();

		//mouse handle
		auto newMousePos = clumsy_engine::Input::get_mouse_position();
		auto mouse_delta = m_drag_delta_computer(newMousePos.x, newMousePos.y, clumsy_engine::Input::is_mouse_button_pressed(CE_MOUSE_BUTTON_LEFT));
		m_camara->dragging_handle(glm::vec2(mouse_delta[0], mouse_delta[1]));

		clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
		clumsy_engine::Render_Command::clear();

		clumsy_engine::Renderer::begin_scene(m_camara);

		plane_update();

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
		ImGui::ColorEdit3("plane color", glm::value_ptr(m_plane_color));

		if (ImGui::Button("open.."))
		{
			auto opend_file = clumsy_engine::File_Dialogs::open_file("mesh file (*.obj)\0*.obj\0 project (*.pro)\0*.pro\0");
			CE_INFO("opened {0}", opend_file);
		}
		if (ImGui::Button("save.."))
		{
			auto saved_file = clumsy_engine::File_Dialogs::save_file("project (*.pro)\0*.pro\0");
			CE_INFO("save {0}", saved_file);
		}

		ImGui::End();
	}


private:


	void config_shader()
	{
		m_positions = {
			0.,0,0,0,0,0.5,
			1,0,0,0,0,0.2,
			1,1,0,0,0,0.2
		};

		std::vector<int> triangles{
			0,1,2
		};

		//shader
		std::string vertex_src = R"(
			#version 330 core
			layout(location=0 ) in vec3 a_position;
			layout(location=1 ) in vec3 a_color;
			out vec3 v_position;
			out vec3 v_color;

			uniform mat4 u_view_projection;
			uniform mat4 u_model_matrix;

			void main()
			{

				v_position=a_position;
				v_color=a_color;
				gl_Position=u_view_projection*u_model_matrix*vec4(a_position,1.0);
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
		m_shader = clumsy_engine::Shader::create(vertex_src, fragment_src);
		auto ogl_shader = std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader > (m_shader);

		//gl data stuff
		m_vertex_array = clumsy_engine::Vertex_Array::create();
		clumsy_engine::Ref<clumsy_engine::Vertex_Buffer > vertex_buffer = clumsy_engine::Vertex_Buffer::create(m_positions.data(), m_positions.size());

		clumsy_engine::Buffer_Layout layout =
		{
			{clumsy_engine::Shader_Data_Type::Float3, "a_position"},
			{clumsy_engine::Shader_Data_Type::Float3, "a_color"}

		};
		vertex_buffer->set_layout(layout);
		m_vertex_array->add_vertex_buffer(vertex_buffer, ogl_shader->get_id());

		clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(triangles.data(), triangles.size());
		m_vertex_array->set_index_buffer(index_buffer);





	}

	void config_shader_plane()
	{

		m_positions_plane = {
			0.,0,0,
			1,0,0,
			1,1,0,
			0,1,0
		};

		std::vector<int> triangles{
			0,1,2,
			0,2,3
		};



		std::string vertex_src = R"(
			#version 330 core
			layout(location=0 ) in vec3 position;

			uniform  vec3 u_color;
			uniform mat4 u_view_projection;
			uniform mat4 u_model_matrix;

			out vec3 v_position;
			out vec3 v_color;

			void main()
			{
				//vec3 t_position=u_view_projection*position;
				v_position=position;
				v_color=u_color;
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
				color=vec4(v_color,1.0);
			}
		)";

		//shader
		m_shader_plane = clumsy_engine::Shader::create(vertex_src, fragment_src);
		auto ogl_shader = std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader > (m_shader_plane);


		//gl data stuff
		m_vertex_array_plane = clumsy_engine::Vertex_Array::create();
		clumsy_engine::Ref<clumsy_engine::Vertex_Buffer > vertex_buffer = clumsy_engine::Vertex_Buffer::create(m_positions_plane.data(), m_positions_plane.size());

		clumsy_engine::Buffer_Layout layout =
		{
			{clumsy_engine::Shader_Data_Type::Float3, "position"},
		};
		vertex_buffer->set_layout(layout);
		m_vertex_array_plane->add_vertex_buffer(vertex_buffer, ogl_shader->get_id());

		clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(triangles.data(), triangles.size());
		m_vertex_array_plane->set_index_buffer(index_buffer);

	}

	void plane_update()
	{
		m_shader_plane->bind();
		std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader>(m_shader_plane)->upload_uniform_vec3("u_color", m_plane_color);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				glm::vec3 delta_pos(0.3f + i * 1.1, 0.3f + j * 1.1, 0.f);
				auto model_transform = glm::translate(glm::mat4(1.f), delta_pos);
				clumsy_engine::Renderer::submit(m_shader_plane, m_vertex_array_plane, model_transform);
			}
		}
	}





	clumsy_engine::Ref<clumsy_engine::Shader> m_shader;
	clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array;
	std::vector<float> m_positions;

	clumsy_engine::Ref<clumsy_engine::Shader> m_shader_plane;
	clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array_plane;
	std::vector<float> m_positions_plane;
	glm::vec3 m_plane_color = glm::vec3(1.f, 0.f, 0.f);

	clumsy_engine::Ref<clumsy_engine::Orthorgraphic_Camara> m_camara;
	clumsy_engine::Drag_Delta_Computer m_drag_delta_computer;

	clumsy_engine::Ref<clumsy_engine::Dispatcher<clumsy_engine::Event, bool>> m_dispatcher;
};


class SanBox_App:public clumsy_engine::Application
{
	public:
		SanBox_App() 
		{
			push_layer(std::make_shared<Layer_Demo>());
		}

};



std::unique_ptr<clumsy_engine::Application> clumsy_engine::create_application()
{

	RECORD_FUNCTION_DURATION();

	clumsy_engine::Log::get_core_logger()->trace("create app");

	std::unique_ptr<clumsy_engine::Application> app = std::make_unique<SanBox_App>(); 
	//std::unique_ptr<clumsy_engine::Application> app = std::make_unique<Sim_App>(); 

	return app;
}


