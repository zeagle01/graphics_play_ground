
#include "sim_app.h"
#include "inertial.h"
#include "gravity.h"
#include "spring_stretch.h"
#include "profiler.h"
#include "file_dialogs.h"
#include "simulator/mesh_loader.h"
#include "openGL_shader.h"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

using namespace clumsy_engine;
	Sim_Gui::Sim_Gui() :
		clumsy_engine::Layer("sim_gui")
		, m_camara(std::make_shared<clumsy_engine::Orthorgraphic_Camara>())
		, m_dispatcher(std::make_shared < clumsy_engine::Dispatcher<clumsy_engine::Event, bool>>())
	{
		float ss = 0.50f;
		m_camara->set_view_field(-ss, ss, -ss, ss, -0.01f, -100.f);
		m_camara->look_at(
			glm::vec3(0.f,0.f,3.f),
			glm::vec3(0.f,0.f,0.f),
			glm::vec3(0.f,1.f,0.f)
		);

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






		std::string vertex_src = R"(
			#version 330 core

			layout(location=0 ) in vec3 v_position;
			layout(location=1 ) in vec3 v_normal;

			uniform mat4 u_view_projection;

			out vec3 frag_normal;
			out vec3 frag_position;

			void main()
			{
				gl_Position=u_view_projection*vec4(v_position,1.0);
				frag_normal=v_normal;
				//frag_position=v_position;
			}

		)";

		std::string fragment_src = R"(
			#version 330 core

			in vec3 frag_normal;
			in vec3 frag_position;

			uniform vec3 u_eye_pos;
			uniform vec3 u_light_pos;
			uniform vec3 u_obj_color;
			uniform vec3 u_light_color;
			uniform float u_ka;
			uniform float u_ks;
			uniform float u_kd;
			uniform float u_specular_steep;

			out vec4 color;

			void main()
			{
				//ambient
				vec3 ambient_color=u_ka*u_light_color*u_obj_color;

				//diffuse
				vec3 light_dir=normalize(u_light_color-frag_position);
				float diffuse=max(dot(frag_normal,light_dir),0.);
				vec3 diffuse_color=u_kd*diffuse*u_light_color*u_obj_color;

				//specular
				vec3 eye_dir=normalize(u_eye_pos-frag_position);
				vec3 specular_dir=normalize(light_dir+eye_dir);
				float specular=pow(max(dot(specular_dir,frag_normal),0),u_specular_steep);
				vec3 specular_color=u_ks*specular*u_light_color*u_obj_color;

				color=vec4(ambient_color+diffuse_color+specular_color,1.0);
				//color=vec4(frag_normal,1.0);
				//color=vec4(1.0,0.,0.,1.0);
			}

		)";

		//shader
		m_shader = clumsy_engine::Shader::create(vertex_src, fragment_src);


		auto ogl_shader = std::dynamic_pointer_cast<OpenGL_Shader>(m_shader);

		//gl data stuff
		m_vertex_array = clumsy_engine::Vertex_Array::create();

		//vbo position
		m_vbo_position = clumsy_engine::Vertex_Buffer::create(m_positions[0].get_flat(), m_positions.size() * 3);
		clumsy_engine::Buffer_Layout vbo_position_layout =
		{
			{clumsy_engine::Shader_Data_Type::Float3, "v_position"}
		};
		m_vbo_position->set_layout(vbo_position_layout);
		m_vertex_array->add_vertex_buffer(m_vbo_position,ogl_shader->get_id());

		//vbo normal
		m_vbo_normal = clumsy_engine::Vertex_Buffer::create(m_normals[0].get_flat(), m_normals.size() * 3);
		clumsy_engine::Buffer_Layout vbo_normal_layout =
		{
			{clumsy_engine::Shader_Data_Type::Float3, "v_normal"}
		};
		m_vbo_normal->set_layout(vbo_normal_layout);
		m_vertex_array->add_vertex_buffer(m_vbo_normal, ogl_shader->get_id());

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


		m_sim.add_interaction<clumsy_engine::Inertial>();
		m_sim.add_interaction<clumsy_engine::Gravity>();
		m_sim.add_interaction<clumsy_engine::Spring_Stretch>();
		//m_sim.add_interaction<clumsy_engine::Collision_EE>();
		simulation_init();
	}

	void Sim_Gui::simulation_init()
	{

		m_sim.set_value<data::Time_Step>(0.01);
		m_sim.set_value<data::Mass_Density>(1.);
		m_sim.set_value<data::Gravity>({ 0,0,0 });
		m_sim.set_value<data::Stretch_Stiff>(1e3f);
		
		m_sim.set_value<clumsy_engine::data::Triangle_Indice>(m_indices);
		m_sim.set_value<clumsy_engine::data::Position>(m_positions);
		m_sim.set_value<clumsy_engine::data::Ref_Position>(m_positions);
		std::vector<vec3f> zero(m_positions.size(), get_uniform<3, 1, float>(0));
		m_sim.set_value<clumsy_engine::data::Velocity>(zero);

	}

	bool Sim_Gui::on_key_pressed(clumsy_engine::Key_Pressed_Event& e)
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
		auto newMousePos = clumsy_engine::Input::get_mouse_position();
		auto mouse_delta = m_drag_delta_computer(newMousePos.x, newMousePos.y, clumsy_engine::Input::is_mouse_button_pressed(CE_MOUSE_BUTTON_LEFT));
		m_camara->dragging_handle(glm::vec2(mouse_delta[0], mouse_delta[1]));

		//render
		clumsy_engine::Renderer::begin_scene(m_camara);


		clumsy_engine::Renderer::submit(m_shader, m_vertex_array,glm::mat4(1.f));

		clumsy_engine::Renderer::end_scene();


		//simulation update
		m_sim.update();
		const auto& new_pos = m_sim.get_value<data::Position>();
		const auto& new_normal = m_sim.get_value<data::Vertex_Normal>();

		//update render data
		m_vbo_position->set_data(new_pos[0].get_flat(), new_pos.size());
		m_vbo_normal->set_data(new_normal[0].get_flat(), new_normal.size());

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

		ImGui::Text("hello world from sim app");

		ImGui::SliderFloat3("gravity", &(m_sim.get_ref_value<data::Gravity>()(0)), -10.f, 10.f);
		ImGui::SliderFloat("time step", &(m_sim.get_ref_value<data::Time_Step>()), 0.001f, 10.f);
		ImGui::SliderFloat("mass density", &(m_sim.get_ref_value<data::Mass_Density>()), 0.001f, 10.f);
		ImGui::SliderFloat("stretch stiff", &(m_sim.get_ref_value<data::Stretch_Stiff>()), 0.001f, 1e7f);

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



