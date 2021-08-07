
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
#include "clumsy_engine/texture.h"
#include "shader_sources.h"

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
		, m_camara(std::make_shared<clumsy_engine::Perspective_Camara>())
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
		config_shader_texture();

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

		//clumsy_engine::Renderer::submit(m_shader, m_vertex_array, glm::mat4(1.f));

		m_texture->bind(m_texture_slot);
		clumsy_engine::Renderer::submit(m_shader_texture, m_vertex_array_texture, glm::scale(glm::mat4(1.f), glm::vec3(4.5f)));


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
			0.,0,0,
			1,0,0,
			1,1,0
		};

		m_colors = {
			0.,1,0.5,
			0,1,0,
			0,1,0
		};

		std::vector<int> triangles{
			0,1,2
		};

		//shader
		m_shader = clumsy_engine::Shader::create(sand_box_triangle_vertex_src, sand_box_triangle_fragment_src);
		auto ogl_shader = std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader > (m_shader);

		//gl data stuff
		int vNum = m_positions.size() / 3;
		m_vertex_array = clumsy_engine::Vertex_Array::create();
		m_vertex_array->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float3,m_position_name_in_shader);
		m_vertex_array->set_vertex_attribute_data(m_position_name_in_shader, m_positions.data(), vNum);
		m_vertex_array->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float3, m_color_name_in_shader);
		m_vertex_array->set_vertex_attribute_data(m_color_name_in_shader, m_colors.data(), vNum);

		clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(triangles.data(), triangles.size());
		m_vertex_array->set_index_buffer(index_buffer);

	}
	void config_shader_texture()
	{
		m_texture_coodinates=
		{
			0,0,
			1.5,0,
			1.5,1.5,
			0,1.5
		};
		m_positions_texture = {
			0.,0,0,
			1,0,0,
			1,1,0,
			0,1,0
		};

		std::vector<int> triangles{
			0,1,2,
			0,2,3
		};

		m_shader_texture = clumsy_engine::Shader::create(sand_box_texture_vertex_src, sand_box_texture_fragment_src);
		auto ogl_shader = std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader > (m_shader_texture);

		m_vertex_array_texture = clumsy_engine::Vertex_Array::create();
		std::string position_name_in_shader = "position";
		std::string texture_name_in_shader = "texture";
		int v_num = m_positions_texture.size() / 3;

		m_vertex_array_texture->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float3, position_name_in_shader);
		m_vertex_array_texture->set_vertex_attribute_data(position_name_in_shader, m_positions_texture.data(), v_num);

		m_vertex_array_texture->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float2, texture_name_in_shader);
		m_vertex_array_texture->set_vertex_attribute_data(texture_name_in_shader, m_texture_coodinates.data(), v_num);

		clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(triangles.data(), triangles.size());
		m_vertex_array_texture->set_index_buffer(index_buffer);

		//texture
		std::string resources_dir = "../../../resources/";
		std::string texture_image = resources_dir + "textures/awesomeface.png";
		m_texture = clumsy_engine::Texture_2D::create(texture_image);
		ogl_shader->bind();
		ogl_shader->upload_uniform_int("u_texture", m_texture_slot);
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

		//shader
		m_shader_plane = clumsy_engine::Shader::create(sand_box_plane_vertex_src, sand_box_plane_fragment_src);
		auto ogl_shader = std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader > (m_shader_plane);


		//gl data stuff
		m_vertex_array_plane = clumsy_engine::Vertex_Array::create();
		std::string position_name_in_shader = "position";
		int v_num = m_positions_plane.size() / 3;

		m_vertex_array_plane->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float3, position_name_in_shader);
		m_vertex_array_plane->set_vertex_attribute_data(position_name_in_shader, m_positions_plane.data(), v_num);

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

	//triangle shader
	clumsy_engine::Ref<clumsy_engine::Shader> m_shader;
	clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array;
	std::vector<float> m_positions;
	std::vector<float> m_colors;
	std::string m_position_name_in_shader="a_position";
	std::string m_color_name_in_shader="a_color";

	//plane shader
	clumsy_engine::Ref<clumsy_engine::Shader> m_shader_plane;
	clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array_plane;
	std::vector<float> m_positions_plane;
	glm::vec3 m_plane_color = glm::vec3(1.f, 0.f, 0.f);

	//texture shader
	clumsy_engine::Ref<clumsy_engine::Shader> m_shader_texture;
	clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array_texture;
	clumsy_engine::Ref<clumsy_engine::Texture> m_texture;
	int m_texture_slot = 0;
	std::vector<float> m_texture_coodinates;
	std::vector<float> m_positions_texture;


	clumsy_engine::Ref<clumsy_engine::Camara> m_camara;
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


