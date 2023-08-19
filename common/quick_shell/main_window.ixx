
module;

#include <memory> 
#include <functional>

export module main_window;

import : UI_wrapper;
import : GLFW_wrapper;


namespace quick_shell
{
	export class  main_window
	{
	public: 
		void init(int width,int height);
		void run_event_loop();

		void register_frame_update_fn(std::function<void()> frame_update_fn);
		void register_frame_update_fn(std::function<void(int, int)> frame_update_fn);

		template<typename Ui>
		void add_ui()
		{
			add_ui_imp<Ui>();
		}

	private:
		GLFW_wrapper  m_glfwWrapper;
	};

}
