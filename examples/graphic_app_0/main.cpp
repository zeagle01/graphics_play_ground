
import main_window;

#include <iostream>

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

	m.register_frame_update_fn(
		[](int cx, int cy) { printf(" %d %d \n", cx, cy); }
	);

	m.run_event_loop();

	return 0;

}