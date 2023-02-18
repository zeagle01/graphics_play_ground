
#include "Spinning_Cube_App.h"

#include "Drawing_Buffer.h"
#include "Imgui_Wrapper.h"
#include "GLFW_Wrapper.h"
#include "Spinning_Cube.h"
#include "ui_data_mapper.h"

#include "mat.h"
#include "member_extractor.h"

namespace soft_render
{

	void Spinning_Cube_App::init()
	{
		type_map::fill_types<config>(m_configs);
		auto& screen_size = m_configs.get_ref<config::screen_size>();


		m_glfw_win = std::make_shared<GLFW_Wrapper>();
		m_drawing_buffer = std::make_shared<Drawing_Buffer>();
		m_gui = std::make_shared<Imgui_Wrapper>();

		m_glfw_win->create_window(screen_size(0), screen_size(1));
		m_drawing_buffer->init(screen_size(0), screen_size(1));
		m_gui->init(m_glfw_win->get_window());


		Spinning_Cube& sc = m_configs.get_ref<config::model>();
		sc.set_spinning_cube_default_value();
	}

	void Spinning_Cube_App::run()
	{

		auto& screen_size = m_configs.get_ref<config::screen_size>();

		Spinning_Cube& sc = m_configs.get_ref<config::model>();
		sc.init(screen_size(0), screen_size(1), m_drawing_buffer.get());

		m_glfw_win->add_event_handler<MousePress>(std::bind(&Spinning_Cube::on_mouse_pressed, &sc, std::placeholders::_1));
		m_glfw_win->add_event_handler<MouseRelease>(std::bind(&Spinning_Cube::on_mouse_released, &sc, std::placeholders::_1));
		m_glfw_win->add_event_handler<Mouse_Scrolled_Event>(std::bind(&Spinning_Cube::on_mouse_scroll, &sc, std::placeholders::_1));

		m_glfw_win->setup_input();

		type_map_view tm;
		tm.add_type<Spinning_Cube>(&sc);
		tm.add_type<Spinning_Cube_App>(this);
		auto camara = sc.m_configs.get_ref<Spinning_Cube::config::camara>();
		tm.add_type<Camara>(&camara);
		add_all_ui::apply(tm, *m_gui);

		m_glfw_win->main_loop([this, &sc](int cursor_x, int cursor_y)
			{
				m_drawing_buffer->clear();

				sc.update(cursor_x, cursor_y);

				m_drawing_buffer->draw();

				m_gui->update();
			}
		);

	}

}
