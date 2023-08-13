
module;

#include <memory> 

export module main_window;

import : UI_wrapper;
import : GLFW_wrapper;


namespace quick_shell
{
	export class  main_window
	{
	public: 
		void init();
		void run_event_loop();

		template<typename Ui>
		void add_ui()
		{
			add_ui_imp<Ui>();
		}

	private:
		std::shared_ptr<GLFW_wrapper > m_glfwWrapper;
	};

}
