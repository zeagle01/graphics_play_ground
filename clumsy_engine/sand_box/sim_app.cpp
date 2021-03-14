
#include "sim_app.h"
#include "inertial.h"
#include "gravity.h"

#include "imgui.h"

using namespace clumsy_engine;
	Sim_Gui::Sim_Gui() :
		clumsy_engine::Layer("sim_gui")
		, m_camara(std::make_shared<clumsy_engine::Orthorgraphic_Camara>(-1,1,-1,1))
		, m_dispatcher(std::make_shared < clumsy_engine::Dispatcher<clumsy_engine::Event, bool>>())
		, m_camara_position(0.f)
		,m_camara_rotation(0.f)
	{


		/// data
		//std::vector<float> positions{
		//	0.,0,0,0,0,0.5,
		//	1,0,0,0,0,0.2,
		//	1,1,0,0,0,0.2
		//};
				std::vector<vec3f> positions{
					{0.,0,0},
					{1,0,0},
					{1,1,0},
				};

		std::vector<int> triangles{
			0,1,2
		};

		//gl data stuff
		m_vertex_array = clumsy_engine::Vertex_Array::create();
		std::shared_ptr<clumsy_engine::Vertex_Buffer > vertex_buffer = clumsy_engine::Vertex_Buffer::create(positions[0].get_flat(), positions.size() * 3);

		clumsy_engine::Buffer_Layout layout =
		{
			{clumsy_engine::Shader_Data_Type::Float3, "a_position"}
			//, {clumsy_engine::Shader_Data_Type::Float3, "a_color"}

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






		///////////////simulation init
		m_sim.add_interaction<clumsy_engine::Inertial>();
		m_sim.add_interaction<clumsy_engine::Gravity>();

		m_sim.set<data::Time_Step>(0.01);
		m_sim.set<data::Mass_Density>(1.);
		m_sim.set<data::Gravity>({ 0,-10,0 });
		
		m_sim.set<clumsy_engine::data::Triangle_Indice>(triangles);
		m_sim.set<clumsy_engine::data::Position>(positions);
		std::vector<vec3f> zero(positions.size(), get_uniform<3, 1, float>(0));
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


		//simulation update
		m_sim.update();
		auto new_pos = m_sim.get<data::Position>();
		m_vertex_array->set_positions(new_pos[0].get_flat(), new_pos.size() / 3);


	};

	void Sim_Gui::on_event(clumsy_engine::Event& e) 
	{
		(*m_dispatcher)(e);
	}

	void Sim_Gui::on_imgui_render(ImGuiContext* imgui_context) 
	{

		ImGui::SetCurrentContext(imgui_context); //cross lib imgui context do not share ! so set it manully

		ImGui::Begin("Test from sim app");

		ImGui::Text("hello world from sim app");

		static float gravity[] = { 0.f,0.f,0.f };
		static float time_step = 0.1f;
		static float rho = 1.f;

		ImGui::SliderFloat3("gravity", gravity, -10.f, 10.f);            
		ImGui::SliderFloat("time step", &time_step, 0.001, 10.f);           
		ImGui::SliderFloat("mass density", &rho, 0.001, 10.f);           

		ImGui::End();

		m_sim.set<data::Time_Step>(time_step);
		m_sim.set<data::Mass_Density>(rho);
		m_sim.set<data::Gravity>({ gravity[0],gravity[1],gravity[2] });

	}



