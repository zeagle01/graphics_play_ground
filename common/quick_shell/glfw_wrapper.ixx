module;

#include <functional>

export module GLFW_Wrapper;


namespace quick_shell
{
	export class GLFW_Wrapper
	{
	public:
		void create_window(int width, int height);
		void main_loop(std::function<void(int cursor_x,int cursor_y)> fn);
	};
}