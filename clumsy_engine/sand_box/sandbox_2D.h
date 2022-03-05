
#pragma once

#include "clumsy_engine/clumsy_engine.h"
#include "clumsy_engine/camara_controller.h"
#include "clumsy_engine/texture.h"


class Sandbox_2D: public clumsy_engine::Layer
{
public:

	Sandbox_2D() :
		clumsy_engine::Layer("sandbox 2D")
		, m_camara_controller(clumsy_engine::new_a_conmara_controller<clumsy_engine::Perspective_Projection>())
	{}

	void on_update(clumsy_engine::Time_Step dt) override;
	void on_imgui_render(ImGuiContext* imgui_context) override;
	void on_event(clumsy_engine::Event& e) override;
	void on_attach() override;
	void on_detach() override;

private:

	clumsy_engine::Ref<clumsy_engine::Camara_Controller> m_camara_controller;

	clumsy_engine::Ref<clumsy_engine::Shader> m_shader_plane;
	clumsy_engine::Ref<clumsy_engine::Vertex_Array> m_vertex_array_plane;
	std::vector<float> m_positions_plane;
	glm::vec4 m_plane_color = glm::vec4(1.f, 0.f, 0.f,1.f);

};
