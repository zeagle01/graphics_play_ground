
import main_window;

#include <iostream>

int main()
{

	quick_shell::main_window m;
	m.init(800,600);

	m.register_frame_update_fn(
		[] { printf(" hello\n"); }
	);

	m.register_frame_update_fn(
		[](int cx, int cy) { printf(" %d %d \n", cx, cy); }
	);

	m.run_event_loop();

	return 0;

}