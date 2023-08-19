
module;

#include <memory> 
#include <functional>

export module main_window;

import : UI_wrapper;
import : GLFW_wrapper;
import : render;


namespace quick_shell
{


	export class renderer
	{
	public:
		void draw_triangles(int* indices, float* pos, int tNum, int vNum);
	private:
		renderer_imp  m_render_imp;
	};


	export class  main_window
	{
	public: 
		// main window
		void init(int width,int height);
		void run_event_loop();
		void register_frame_update_fn(std::function<void()> frame_update_fn);
		void register_frame_update_fn(std::function<void(int, int)> frame_update_fn);

		//
		renderer& get_renderer() { return m_renderer; }


		template<typename Ui>
		void add_ui()
		{
			add_ui_imp<Ui>();
		}

	private:
		GLFW_wrapper  m_glfwWrapper;
		renderer m_renderer;
	};

}
