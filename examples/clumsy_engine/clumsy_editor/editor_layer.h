
#pragma once

#include "clumsy_engine/clumsy_engine.h"
#include "clumsy_engine/camara_controller.h"
#include "clumsy_engine/texture.h"
#include "clumsy_engine/frame_buffer.h"

namespace clumsy_engine
{
	class editor_layer : public Layer
	{
	public:

		editor_layer() :
			Layer("sandbox 2D"),
			m_camara_controller(clumsy_engine::new_a_conmara_controller<Orthographic_Projection>())
		{}

		void on_update(Time_Step dt) override;
		void on_imgui_render(ImGuiContext* imgui_context) override;
		void on_event(Event& e) override;
		void on_attach() override;
		void on_detach() override;

	private:

		Ref<Camara_Controller> m_camara_controller;

		Ref<Texture_2D> m_texture;
		Ref<frame_buffer> m_frame_buffer;

		glm::vec4 m_plane_color = glm::vec4(1.f, 0.f, 0.f, 1.f);
		glm::vec2 m_view_port_size;

	};
}

