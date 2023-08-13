
module;

#include <memory> 

export module Main_Window;


//import GLFW_Wrapper;

import :par;


namespace quick_shell
{
	class GLFW_Wrapper;

	export class  Main_Window
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
		std::shared_ptr<GLFW_Wrapper > m_glfwWrapper;
	};

}
