
#pragma once

#include "clumsy_engine/clumsy_engine.h"
#include "simulator/Simulator.h"
#include "clumsy_engine/ref.h"
#include "ui_map_to_simulation_records.h"


#include <memory>


class Sim_Gui :public clumsy_engine::Layer
{
public:
	Sim_Gui() ;
	

	bool on_key_pressed(clumsy_engine::Key_Pressed_Event& e);

	void on_attach()  override;

	void on_detach()override ;
	void on_update(clumsy_engine::Time_Step dt) override;

	virtual void on_event(clumsy_engine::Event& e);
	
	bool on_mouse_scroll(clumsy_engine::Mouse_Scrolled_Event& e);

	virtual void on_imgui_render(ImGuiContext* imgui_context);
	

private:
	void simulation_init();

	clumsy_engine::Ref<clumsy_engine::Shader> m_shader;
	clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array;
	std::string m_attribute_name_position = "v_position";
	std::string m_attribute_name_normal = "v_normal";

	std::vector<vec3f> m_positions;
	std::vector<vec3f> m_normals;
	std::vector<int> m_indices;

	clumsy_engine::Ref<clumsy_engine::Camara> m_camara;

	clumsy_engine::Drag_Delta_Computer m_drag_delta_computer;

	clumsy_engine::Ref<clumsy_engine::Dispatcher<clumsy_engine::Event, bool>> m_dispatcher;

	clumsy_engine::Simulator m_sim;


	clumsy_lib::Type_Map<Simulation_Data_Mapper_Base> m_simulation_mappers;

};


class Sim_App:public clumsy_engine::Application
{
	public:
		Sim_App() 
		{

			push_layer(std::make_shared<Sim_Gui>());
		}

};
