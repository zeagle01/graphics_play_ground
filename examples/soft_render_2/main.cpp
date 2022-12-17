

#include "Drawing_Buffer.h"
#include "Imgui_Wrapper.h"
#include "Spinning_Cube.h"
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

	//TODO:
	//connect(sc_p0, ui_component0)
	//connect(sc_p1, ui_component1)
	//...

	//std::function<soft_render::vec3& ()> fn = [&sc]()->soft_render::vec3& {return sc.get_config<config::angle_rate>(); };
	vec3& v = sc.get_config<config::angle_rate>();
	imgui_wrapper.add_ui_component<gui_component::slider_bar_float3>(v);


	screen.main_loop([&screen, &sc, &imgui_wrapper, width, height ]()
		{
			screen.clear();

			sc.update();

			imgui_wrapper.update();

		});

	return 0;
}


