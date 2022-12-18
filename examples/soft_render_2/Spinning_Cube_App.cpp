
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

	void Spinning_Cube_App::run()
	{

		for_each_type<extract_member_type_list_t<config>>::apply<init_app>(m_configs);

		Drawing_Buffer screen;
		Imgui_Wrapper imgui_wrapper;

		int width = 800;
		int height = 600;

		screen.init(width, height);
		imgui_wrapper.init(screen.get_window());

		Spinning_Cube& sc = m_configs.get_ref<config::model>();
		sc.init(width, height, &screen);

		add_all_ui_components(sc, imgui_wrapper);

		screen.main_loop([&screen, &sc, &imgui_wrapper]()
			{
				screen.clear();

				sc.update();

				imgui_wrapper.update();

			});

	}

}
