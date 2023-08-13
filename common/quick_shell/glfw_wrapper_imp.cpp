
module GLFW_Wrapper;



namespace quick_shell
{

	void GLFW_Wrapper::create_window(int width, int height)
	{

	}

	void GLFW_Wrapper::main_loop(std::function<void(int cursor_x, int cursor_y)> fn)
	{
		while (true)
		{
			fn(0, 0);
		}

	}
}
