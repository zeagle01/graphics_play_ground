
#include "Spinning_Cube_App.h"

#include "Drawing_Buffer.h"
#include "Imgui_Wrapper.h"
#include "Spinning_Cube.h"
#include "ui_data_mapper.h"

#include "mat.h"
#include "member_extractor.h"

namespace soft_render
{

	void Spinning_Cube_App::init()
	{
		type_map::fill_types<config>(m_configs);

		m_drawing_buffer = std::make_shared<Drawing_Buffer>();
		m_gui = std::make_shared<Imgui_Wrapper>();

		auto& screen_size = m_configs.get_ref<config::screen_size>();

		m_drawing_buffer->init(screen_size(0), screen_size(1));
		m_gui->init(m_drawing_buffer->get_window());


		Spinning_Cube& sc = m_configs.get_ref<config::model>();
		sc.set_spinning_cube_default_value();
	}

	void Spinning_Cube_App::run()
	{

		auto& screen_size = m_configs.get_ref<config::screen_size>();

		Spinning_Cube& sc = m_configs.get_ref<config::model>();
		sc.init(screen_size(0), screen_size(1), m_drawing_buffer.get());

		type_map_view tm;
		tm.add_type<Spinning_Cube>(&sc);
		tm.add_type<Spinning_Cube_App>(this);
		add_all_ui::apply(tm, *m_gui);

		m_drawing_buffer->main_loop([this, &sc](int cursor_x, int cursor_y)
			{
				m_drawing_buffer->clear();

				sc.update(cursor_x, cursor_y);

				m_gui->update();

			});

	}

}
