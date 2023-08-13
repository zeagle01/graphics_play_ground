
module;

#include <functional>
#include <iostream>

module main_window:GLFW_wrapper;


namespace quick_shell
{
	class GLFW_wrapper
	{
	public:
		void create_window(int width, int height)
		{
			printf(" creted %d %d \n", width, height);

		}
		void main_loop(std::function<void(int cursor_x, int cursor_y)> fn)
		{

			while (true)
			{
				fn(0, 0);
			}
		}
	};
}
