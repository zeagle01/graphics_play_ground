
export module Main_Window;

import :par;

namespace quick_shell
{
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
	};

}
