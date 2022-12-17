

#include "Drawing_Buffer.h"
#include "Imgui_Wrapper.h"
#include "Spinning_Cube.h"
#include "ui_data_mapper.h"
#include "mat.h"

using namespace soft_render;

int main()
{
	Drawing_Buffer screen;
	Imgui_Wrapper imgui_wrapper;

	int width = 800;
	int height = 600;

	screen.init(width, height);
	imgui_wrapper.init(screen.get_window());

	Spinning_Cube sc(width, height, &screen);

	add_all_ui_components(sc, imgui_wrapper);

	screen.main_loop([&screen, &sc, &imgui_wrapper, width, height ]()
		{
			screen.clear();

			sc.update();

			imgui_wrapper.update();

		});

	return 0;
}


