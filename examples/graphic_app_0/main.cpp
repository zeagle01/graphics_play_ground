
import main_window;

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

	float t = 0.f;
	float step = 0.01f;
	auto animation_fn = [&]()
		{
			std::ranges::for_each(pos, [&](auto& x) {x += 0.001f * (std::sin(t) - 0.5f); });
			t += step;
			printf(" %f \n", t);
		};

	m.register_frame_update_fn(render_fn);

	m.register_frame_update_fn(animation_fn);

	m.add_ui_component<ui_component::check_box>([&](const auto& v) {step = float(v) * step; });

	m.run_event_loop();

	return 0;

}