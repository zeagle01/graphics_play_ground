

#include "Drawing_Buffer.h"
#include "Imgui_Wrapper.h"
#include "Spinning_Cube.h"


int main()
{
	Drawing_Buffer screen;
	Imgui_Wrapper imgui_wrapper;

	int width = 800;
	int height = 600;

	screen.init(width, height);
	imgui_wrapper.init(screen.get_window());
	float a = 0.5f;
	imgui_wrapper.add_slider_bar([&a]()->float& {return a; });

	Spinning_Cube sc{ width,height, &screen };

	screen.main_loop([&screen, &sc, &imgui_wrapper, width, height, &a]()
		{
			screen.clear();

			sc.update();

			imgui_wrapper.update();

		});

	return 0;
}


