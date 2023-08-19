
import main_window;

#include <iostream>
#include <ranges>
#include <algorithm>

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
	

	m.register_frame_update_fn(
		[&]
		{
			renderer.draw_triangles(indices, pos, 1, 3);
		}
	);

	float t = 0.f;
	float step = 0.01f;
	m.register_frame_update_fn(
		[&](int cx, int cy) 
		{
			std::ranges::for_each(pos, [&](auto& x) {x += 0.001f * (std::sin(t) - 0.5f); });
			t += step;
			printf(" %f \n", t);
		}
	);

	m.run_event_loop();

	return 0;

}