
#include "sim_app.h"
#include "inertial.h"
#include "gravity.h"
#include "spring_stretch.h"
#include "profiler.h"
#include "file_dialogs.h"
#include "simulator/mesh_loader.h"
#include "openGL_shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "clumsy_engine/mouse_event.h"
#include "shader_sources.h"

#include "imgui.h"

using namespace clumsy_engine;
	Sim_Gui::Sim_Gui() :
		clumsy_engine::Layer("sim_gui")
		, m_camara(clumsy_engine::new_a_camara<clumsy_engine::View_Handler, clumsy_engine::Perspective_Projection>())
		, m_dispatcher(std::make_shared < clumsy_engine::Dispatcher<clumsy_engine::Event, bool>>())
	{
		float ss = 0.50f;
		m_camara->set_view_field(-ss, ss, -ss, ss, -0.01f, -100.f);
		m_camara->set_look_at(
			glm::vec3(0.f,0.f,3.f),
			glm::vec3(0.f,0.f,0.f),
			glm::vec3(0.f,1.f,0.f)
		);


		m_dispatcher->add<clumsy_engine::Mouse_Scrolled_Event>(std::bind(&Sim_Gui::on_mouse_scroll, this, std::placeholders::_1));


		m_positions = {
			{0.,0,0},
			{0.5,0,0},
			{0.5,0.5,0},
		};

		m_normals = {
			{0.f,0.f,1.f},
			{0.f,0.f,1.f},
			{0.f,0.f,1.f}
		};

		m_indices = {
					0,1,2
		};




		//shader

		std::string resources_dir = "../../../resources/";
		m_shader = clumsy_engine::Shader::create(resources_dir + "shaders/cloth_sim.glsl");


		auto ogl_shader = std::dynamic_pointer_cast<OpenGL_Shader>(m_shader);

		//gl data stuff
		m_vertex_array = clumsy_engine::Vertex_Array::create();
		int v_num = m_positions.size();


		m_vertex_array->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float3, m_attribute_name_position);
		m_vertex_array->set_vertex_attribute_data(m_attribute_name_position, &m_positions[0](0), v_num);

		//vbo normal
		m_vertex_array->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float3, m_attribute_name_normal);
		m_vertex_array->set_vertex_attribute_data(m_attribute_name_normal, &m_normals[0](0), v_num);

		//ibo 
		clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(m_indices.data(), m_indices.size());
		m_vertex_array->set_index_buffer(index_buffer);

		ogl_shader->bind();
		ogl_shader->upload_uniform_vec3("u_light_pos", glm::vec3(10.f, 10.f, 10.f));
		ogl_shader->upload_uniform_vec3("u_obj_color", glm::vec3(1.f, 0.f, 0.f));
		ogl_shader->upload_uniform_vec3("u_eye_pos", m_camara->get_position());
		ogl_shader->upload_uniform_vec3("u_light_color", glm::vec3(1.f, 1.f, 1.f));
		ogl_shader->upload_uniform_float("u_ka", 0.3f);
		ogl_shader->upload_uniform_float("u_ks", 0.5f);
		ogl_shader->upload_uniform_float("u_kd", 0.5f);
		ogl_shader->upload_uniform_float("u_specular_steep", 40.f);



		auto key_pressed_handler = [](auto& e)
		{
			return false;
		};


		//simulation data mapper
		m_simulation_mappers.add_types<UI_Simulation_Mapper>();

		simulation_init();


	}

	void Sim_Gui::simulation_init()
	{

		for (auto& mapper : m_simulation_mappers)
		{
			//auto mapper = it.second;
			mapper->set_to_default_value(&m_sim);
		}

		m_sim.set_value<clumsy_engine::data::Triangle_Indice>(m_indices);
		m_sim.set_value<clumsy_engine::data::Position>(m_positions);
		m_sim.set_value<clumsy_engine::data::Ref_Position>(m_positions);
		m_sim.get_ref_value<clumsy_engine::data::Velocity>() = std::vector<vec3f>(m_positions.size(), get_uniform<3, 1, float>(0));

	}

	bool Sim_Gui::on_mouse_scroll(clumsy_engine::Mouse_Scrolled_Event& e)
	{
		m_camara->zoom(e.get_y_offset());
		return true;
	}


	void Sim_Gui::on_attach()  
	{
	};

	void Sim_Gui::on_detach() 
	{
	};

	void Sim_Gui::on_update(clumsy_engine::Time_Step dt) 
	{

		RECORD_FUNCTION_DURATION();

		clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
		clumsy_engine::Render_Command::clear();

		//mouse handle
		m_drag_delta_computer.compute();
		{
			auto mouse_delta = m_drag_delta_computer.get(CE_MOUSE_BUTTON_LEFT);
			m_camara->rotate(glm::vec2(mouse_delta[0], mouse_delta[1]));
		}
		{
			auto mouse_delta = m_drag_delta_computer.get(CE_MOUSE_BUTTON_RIGHT);
			m_camara->translate(glm::vec2(mouse_delta[0], mouse_delta[1]));
		}

		//render
		clumsy_engine::Renderer::begin_scene(m_camara);

		clumsy_engine::Renderer::submit(m_shader, m_vertex_array,glm::mat4(1.f));

		clumsy_engine::Renderer::end_scene();


		//simulation update
		m_sim.update();
		const auto& new_pos = m_sim.get_value<data::Position>();
		const auto& new_normal = m_sim.get_value<data::Vertex_Normal>();

		//update render data
		m_vertex_array->set_vertex_attribute_data(m_attribute_name_position, new_pos[0].get_flat(), new_pos.size());
		m_vertex_array->set_vertex_attribute_data(m_attribute_name_normal, new_normal[0].get_flat(), new_normal.size());

		clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(m_indices.data(), m_indices.size());
		m_vertex_array->set_index_buffer(index_buffer);


	};

	void Sim_Gui::on_event(clumsy_engine::Event& e) 
	{
		RECORD_FUNCTION_DURATION();
		(*m_dispatcher)(e);
	}

	void Sim_Gui::on_imgui_render(ImGuiContext* imgui_context) 
	{
		RECORD_FUNCTION_DURATION();

		ImGui::SetCurrentContext(imgui_context); //cross lib imgui context do not share ! so set it manully

		ImGui::Begin("Test from sim app");


		if (ImGui::Button("open.."))
		{
			Mesh_Loader loader;
			auto opend_file = clumsy_engine::File_Dialogs::open_file("mesh file (*.obj)\0*.obj\0 project (*.pro)\0*.pro\0");
			if (!opend_file.empty())
			{
				loader.load_from_obj(opend_file);
				const auto& new_pos = loader.get_positions();
				int v_num = new_pos.size() / 3;
				m_positions.resize(v_num);
				for (int i = 0; i < v_num; i++)
				{
					m_positions[i](0) = new_pos[i * 3 + 0];
					m_positions[i](1) = new_pos[i * 3 + 1];
					m_positions[i](2) = new_pos[i * 3 + 2];
				}

				m_indices = loader.get_indices();
				simulation_init();
				CE_INFO("opened {0}", opend_file);
			}
		}
		if (ImGui::Button("save.."))
		{
			auto saved_file = clumsy_engine::File_Dialogs::save_file("project (*.pro)\0*.pro\0");
			CE_INFO("save {0}", saved_file);
		}

		ImGui::Text("---begin simulation panel");
		for (auto& mapper : m_simulation_mappers)
		{
			//auto mapper = it.second;
			mapper->update_from_ui(&m_sim);
		}
		ImGui::Text("---end simulation panel");


		//set render uniform
		auto ogl_shader = std::dynamic_pointer_cast<OpenGL_Shader>(m_shader);
		ogl_shader->bind();
		static glm::vec3 light_pos = glm::vec3(10.f, 10.f, 10.f);
		static glm::vec3 obj_color = glm::vec3(1.f, 0.f, 0.f);
		static glm::vec3 light_color = glm::vec3(1.f, 1.f, 1.f);
		static float ka = 0.3f;
		static float ks = 0.5f;
		static float kd = 0.5f;
		static float specular_steep = 0.5f;

		ogl_shader->upload_uniform_vec3("u_eye_pos", m_camara->get_position());

		if (ImGui::SliderFloat3("light pos", glm::value_ptr(light_pos), -1e2f, 1e2f))
		{
			ogl_shader->upload_uniform_vec3("u_light_pos", light_pos);
		}
		if (ImGui::SliderFloat3("obj_color", glm::value_ptr(obj_color), 0.f, 1.f))
		{
			ogl_shader->upload_uniform_vec3("u_obj_color", obj_color);
		}
		if (ImGui::SliderFloat3("light_color", glm::value_ptr(light_color), 0.f, 1.f))
		{
			ogl_shader->upload_uniform_vec3("u_light_color", light_color);
		}
		if (ImGui::SliderFloat("ambient_coefficient", &ka, 0.f, 1.f))
		{
			ogl_shader->upload_uniform_float("u_ka", ka);
		}
		if (ImGui::SliderFloat("diffusion_coefficient", &kd, 0.f, 1.f))
		{
			ogl_shader->upload_uniform_float("u_kd", kd);
		}
		if (ImGui::SliderFloat("specular_coefficient", &ks, 0.f, 1.f))
		{
			ogl_shader->upload_uniform_float("u_ks", ks);
		}
		if (ImGui::SliderFloat("specular_steep", &specular_steep, 1e-2f, 1e2f))
		{
			ogl_shader->upload_uniform_float("u_specular_steep", specular_steep);
		}

		ImGui::End();



	}



