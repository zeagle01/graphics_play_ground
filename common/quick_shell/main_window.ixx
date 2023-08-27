
module;

#include <memory> 
#include <functional>
#include <ranges>
#include <algorithm>

export module quick_shell;

import : UI_wrapper;
import : GLFW_wrapper;
import : render;

export import :ui_components; 

namespace quick_shell
{
	export class renderer
	{
	public:
		template<typename proc>
		void load_glad(proc* proc_addr)
		{
			m_render_imp.load_glad(proc_addr);
		}

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

		// expose render api obj
		renderer& get_renderer() { return m_renderer; }

		//ui
		template<typename  ui_com>
		void add_ui_component(const std::string& name, typename ui_com::type& value, const typename ui_com::extra_data& extra_d,
			std::function<void(const typename ui_com::type&)> call_back = nullptr)
		{
			m_ui_panel.add_ui_component<ui_com>(name, value, extra_d, call_back);
		}

		template<typename  ui_com>
		void add_ui_component(const std::string& name, typename ui_com::type& value)
		{
			m_ui_panel.add_ui_component<ui_com>(name, value, no_extra{}, nullptr);
		}

	private:
		GLFW_wrapper m_glfwWrapper;
		renderer m_renderer;
		ui_panel m_ui_panel;
		std::function<void()> m_ui_fn;
	};

}
