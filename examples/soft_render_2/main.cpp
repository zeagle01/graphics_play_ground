

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

	Spinning_Cube sc{ width,height, &screen };

	//TODO:
	//connect(sc_p0, ui_component0)
	//connect(sc_p1, ui_component1)
	//...
	imgui_wrapper.add_slider_bar([&sc]()->float& {return sc.get_config<Spinning_Cube::angle_rate_x>(); });
	imgui_wrapper.add_slider_bar([&sc]()->float& {return sc.get_config<Spinning_Cube::angle_rate_y>(); });
	imgui_wrapper.add_slider_bar([&sc]()->float& {return sc.get_config<Spinning_Cube::angle_rate_z>(); });

	screen.main_loop([&screen, &sc, &imgui_wrapper, width, height ]()
		{
			screen.clear();

			sc.update();

			imgui_wrapper.update();

		});

	return 0;
}


