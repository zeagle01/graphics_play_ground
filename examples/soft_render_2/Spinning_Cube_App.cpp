
#include "Spinning_Cube_App.h"

#include "Drawing_Buffer.h"
#include "Imgui_Wrapper.h"
#include "Spinning_Cube.h"
#include "ui_data_mapper.h"

#include "mat.h"
#include "member_extractor.h"

namespace soft_render
{

	struct init_app
	{
		template<typename T>
		static void apply(type_map& tm)
		{
			tm.add_type<T>();
		}

	};

	void Spinning_Cube_App::init()
	{

		m_drawing_buffer = std::make_shared<Drawing_Buffer>();
		m_gui = std::make_shared<Imgui_Wrapper>();


		m_drawing_buffer->init(m_width, m_height);
		m_gui->init(m_drawing_buffer->get_window());

		for_each_type<extract_member_type_list_t<config>>::apply<init_app>(m_configs);

		Spinning_Cube& sc = m_configs.get_ref<config::model>();
		sc.set_spinning_cube_default_value();
	}

	void Spinning_Cube_App::run()
	{

		Spinning_Cube& sc = m_configs.get_ref<config::model>();
		sc.init(m_width, m_height, m_drawing_buffer.get());

		add_all_ui::apply(sc, *m_gui);

		m_drawing_buffer->main_loop([this, &sc]()
			{
				m_drawing_buffer->clear();

				sc.update();

				m_gui->update();

			});

	}

}
