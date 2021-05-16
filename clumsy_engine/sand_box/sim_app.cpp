
#include "sim_app.h"
#include "inertial.h"
#include "gravity.h"
#include "spring_stretch.h"
#include "profiler.h"
#include "file_dialogs.h"
#include "simulator/mesh_loader.h"

#include "imgui.h"

using namespace clumsy_engine;
	Sim_Gui::Sim_Gui() :
		clumsy_engine::Layer("sim_gui")
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
		//std::vector<float> positions{
		//	0.,0,0,0,0,0.5,
		//	1,0,0,0,0,0.2,
		//	1,1,0,0,0,0.2
		//};
		m_positions = {
			{0.,0,0},
			{0.5,0,0},
			{0.5,0.5,0},
		};
		m_indices = {
					0,1,2
		};

		//gl data stuff
		m_vertex_array = clumsy_engine::Vertex_Array::create();
		clumsy_engine::Ref<clumsy_engine::Vertex_Buffer > vertex_buffer = clumsy_engine::Vertex_Buffer::create(m_positions[0].get_flat(), m_positions.size() * 3);

		clumsy_engine::Buffer_Layout layout =
		{
			{clumsy_engine::Shader_Data_Type::Float3, "a_position"}
			//, {clumsy_engine::Shader_Data_Type::Float3, "a_color"}

		};
		vertex_buffer->set_layout(layout);
		m_vertex_array->add_vertex_buffer(vertex_buffer);

		clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(m_indices.data(), m_indices.size());
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
		m_shader = clumsy_engine::Shader::create(vertex_src, fragment_src);


		auto key_pressed_handler = [](auto& e)
		{
			return false;
		};


		simulation_init();
	}

	void Sim_Gui::simulation_init()
	{
		m_sim.add_interaction<clumsy_engine::Inertial>();
		m_sim.add_interaction<clumsy_engine::Gravity>();
		m_sim.add_interaction<clumsy_engine::Spring_Stretch>();

		m_sim.set<data::Time_Step>(0.01);
		m_sim.set<data::Mass_Density>(1.);
		m_sim.set<data::Gravity>({ 0,0,0 });
		m_sim.set<data::Stretch_Stiff>(1e3f);
		
		m_sim.set<clumsy_engine::data::Triangle_Indice>(m_indices);
		m_sim.set<clumsy_engine::data::Position>(m_positions);
		m_sim.set<clumsy_engine::data::Ref_Position>(m_positions);
		std::vector<vec3f> zero(m_positions.size(), get_uniform<3, 1, float>(0));
		m_sim.set<clumsy_engine::data::Velocity>(zero);

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

		clumsy_engine::Renderer::begin_scene(m_camara);

		clumsy_engine::Renderer::submit(m_shader, m_vertex_array,glm::mat4(1.f));

		clumsy_engine::Renderer::end_scene();


		//simulation update
		m_sim.update();
		auto new_pos = m_sim.get<data::Position>();
		m_vertex_array->set_positions(new_pos[0].get_flat(), new_pos.size() );


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

		static float gravity[] = { 0.f,-10.f,0.f };
		static float time_step = 0.01f;
		static float rho = 1.f;
		static float stretch_stiff = 1e2f;

		ImGui::SliderFloat3("gravity", gravity, -10.f, 10.f);            
		ImGui::SliderFloat("time step", &time_step, 0.001, 10.f);           
		ImGui::SliderFloat("mass density", &rho, 0.001, 10.f);           
		ImGui::SliderFloat("stretch stiff", &stretch_stiff, 0.001, 1e7f);           


		ImGui::End();


		m_sim.set<data::Time_Step>(time_step);
		m_sim.set<data::Mass_Density>(rho);
		m_sim.set<data::Gravity>({ gravity[0],gravity[1],gravity[2] });
		m_sim.set<data::Stretch_Stiff>(stretch_stiff);

	}



