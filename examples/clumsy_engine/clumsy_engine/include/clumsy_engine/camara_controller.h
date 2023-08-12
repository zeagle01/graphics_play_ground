
#pragma once

#include "camara.h"
#include "ref.h"
#include "time_step.h"
#include "mouse_event.h"
#include "application_event.h"
#include "input.h"

#include <memory>


namespace clumsy_engine
{

	class Camara_Controller
	{
	public:
		Camara_Controller(std::shared_ptr<Projection_Handler> projection_handler);

		void set_aspect_ratio(float aspect_ratio);

		void on_update(Time_Step ts);
		void on_event(Event& e);

		clumsy_engine::Ref<clumsy_engine::Camara> get_camara() { return m_camara; }

	private:
		bool on_mouse_scrolled(Mouse_Scrolled_Event& e);
		bool on_window_resized(Window_Resize_Event& e);

	private:

		float m_aspect_ratio;
		float m_zoom_level = 1.f;
		float const m_near_plane = -1;
		float const m_far_plane = -1000.f;

		clumsy_engine::Ref<clumsy_engine::Camara> m_camara;

		clumsy_engine::Drag_Delta_Computer m_drag_delta_computer;

	};

	template<typename Projection_H>
	static std::shared_ptr<Camara_Controller> new_a_conmara_controller() { return std::make_shared< Camara_Controller>( std::make_shared<Projection_H>()); };
}

