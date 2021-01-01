
#pragma once

#include "clumsy_engine/clumsy_engine.h"
#include <memory>
#include "simulator/Simulator.h"


class Sim_Gui :public clumsy_engine::Layer
{
public:
	Sim_Gui() ;
	

	bool on_key_pressed(clumsy_engine::Key_Pressed_Event& e);

	void on_attach()  override;

	void on_detach()override ;
	void on_update(clumsy_engine::Time_Step dt) override;

	virtual void on_event(clumsy_engine::Event& e);
	

	virtual void on_imgui_render(ImGuiContext* imgui_context);
	

private:

	std::shared_ptr<clumsy_engine::Shader> m_shader;
	std::shared_ptr<clumsy_engine::Vertex_Array> m_vertex_array;

	std::shared_ptr<clumsy_engine::Orthorgraphic_Camara> m_camara;

	glm::vec3 m_camara_position;

	float m_camara_rotation;

	std::shared_ptr<clumsy_engine::Dispatcher<clumsy_engine::Event, bool>> m_dispatcher;

	clumsy_engine::Simulator m_sim;
};


class Sim_App:public clumsy_engine::Application
{
	public:
		Sim_App() 
		{

			//std::shared_ptr<clumsy_engine::Layer> layer = std::make_shared<Layer_Demo>();
			push_layer(std::make_shared<Sim_Gui>());
		}

};
