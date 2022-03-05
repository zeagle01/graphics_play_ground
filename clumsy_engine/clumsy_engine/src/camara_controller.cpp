

#include "camara_controller.h"
#include "dispatcher.h"

namespace clumsy_engine
{

	Camara_Controller::Camara_Controller(std::shared_ptr<Projection_Handler> projection_handler) :
		m_camara(std::make_shared<Camara>(std::make_shared<View_Handler>(), projection_handler))
	{

	}

	void Camara_Controller::set_aspect_ratio(float aspect_ratio)
	{

		m_aspect_ratio = aspect_ratio;

		m_camara->set_view_field(-m_zoom_level * m_aspect_ratio, m_zoom_level * m_aspect_ratio, -m_zoom_level, m_zoom_level, m_near_plane, m_far_plane);

		m_camara->set_look_at(
			glm::vec3(0.f,0.f,10.f),
			glm::vec3(0.f,0.f,0.f),
			glm::vec3(0.f,1.f,0.f)
		);

	}

	void Camara_Controller::on_update(Time_Step ts)
	{
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

	}

	void Camara_Controller::on_event(Event& e)
	{
		clumsy_engine::Dispatcher<clumsy_engine::Event, bool> dispatcher;
		dispatcher.add<clumsy_engine::Mouse_Scrolled_Event>(std::bind(&Camara_Controller::on_mouse_scrolled, this, std::placeholders::_1));
		dispatcher.add<clumsy_engine::Window_Resize_Event>(std::bind(&Camara_Controller::on_window_resized, this, std::placeholders::_1));

		dispatcher(e);
	}

	bool Camara_Controller::on_mouse_scrolled(Mouse_Scrolled_Event& e)
	{
		m_camara->zoom(e.get_y_offset());
		return true;

	}
	bool Camara_Controller::on_window_resized(Window_Resize_Event& e)
	{
		m_aspect_ratio = e.get_width() / e.get_height();

		m_camara->set_view_field(-m_zoom_level * m_aspect_ratio, m_zoom_level * m_aspect_ratio, -m_zoom_level, m_zoom_level, m_near_plane, m_far_plane);

		return true;
	}
}

