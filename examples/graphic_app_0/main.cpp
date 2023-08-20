
import main_window;
import sim_lib;

#include <iostream>
#include <ranges>
#include <algorithm>

using namespace quick_shell;

int main()
{

	quick_shell::main_window m;
	m.init(800,600);

	auto& renderer = m.get_renderer();

	int indices[] = { 0,1,2 };
	float pos[] = {
		0.f,0.f,0.f,
		1.f,0.f,0.f,
		1.f,1.f,0.f
	};
	
	auto render_fn = [&]
		{
			renderer.draw_triangles(indices, pos, 1, 3);
		};



	sim_lib::simulator sim;
	sim.start();

	float t = 0.f;
	float step = 0.01f;
	bool move = true;
	auto animation_fn = [&]()
		{
			if (move)
			{
				std::ranges::for_each(pos, [&](auto& x) {x += 0.001f * (std::sin(t) - 0.5f); });
				t += step;
			}
		};

	// update fn
	m.register_frame_update_fn(render_fn);

	m.register_frame_update_fn(animation_fn);

	//ui panel
	m.add_ui_component<ui_component::check_box>("enable animation", move );
	m.add_ui_component<ui_component::slider_bar>("time step", step, { 0.f,0.1f });

	std::array<float, 2> vec2;
	std::array<float, 3> vec3;

	m.add_ui_component<ui_component::slider_bar2>("vec2 ", vec2, { 0.f,1.f });
	m.add_ui_component<ui_component::slider_bar3>("vec3", vec3, { 0.f,1.f });

	std::string name = "adfasdf";
	m.add_ui_component<ui_component::text_line>("fps", name);

	m.run_event_loop();

	return 0;

}