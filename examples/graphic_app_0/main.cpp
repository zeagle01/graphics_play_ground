
import quick_shell;
import sim_lib;

#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>

using namespace quick_shell;

int main()
{

	quick_shell::main_window m;
	m.init(800,600);

	auto& renderer = m.get_renderer();

	int indices[] = { 0,1,2 };
	std::vector<float> pos = {
		0.f,0.f,0.f,
		1.f,0.f,0.f,
		1.f,1.f,0.f
	};
	
	auto render_fn = [&]
		{
			renderer.draw_triangles(indices, pos.data(), 1, 3);
		};


	sim_lib::simulator sim;

	sim.set<sim_lib::sim_data::positions>(pos);

	auto animation_fn = [&]()
		{
			sim.step();
			pos = sim.get<sim_lib::sim_data::positions>();
		};



	// update fn
	m.register_frame_update_fn(render_fn);
	m.register_frame_update_fn(animation_fn);

	m.run_event_loop();

	return 0;

}