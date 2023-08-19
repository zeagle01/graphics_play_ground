module;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <functional>
#include <unordered_map>
#include <memory> 
#include <ranges>
#include <algorithm>


module main_window : UI_wrapper;

import : ui_components;

namespace quick_shell
{
	class ui_component_base
	{
	public:
		~ui_component_base() {};
	};


	template<typename ui_com>
	class ui_component_imp;

	template<>
	class ui_component_imp<ui_component::check_box> :public ui_component_base
	{
	public:
		bool is_changed() 
		{
			return ImGui::Checkbox("check_box_aaa", &m_value);
		}
		const bool& get_value() { return m_value; }
	private:
		bool m_value;
	};

	class ui_panel
	{
	public:
		template<typename  ui_com>
		void add_ui_component(std::function<void( const typename ui_com::type&)> slot)
		{
			auto ui_com_obj = std::make_shared<ui_component_imp<ui_com>>();

			ui_component_imp<ui_component::check_box>& ui_com_obj_tmp = *ui_com_obj;
			auto wraped_slot = [&,slot]() 
			{
				if (ui_com_obj_tmp.is_changed())
				{
					slot(ui_com_obj_tmp.get_value());
				}
			};
			std::vector<std::function<void()>> slots{wraped_slot};
			m_connects.insert({ ui_com_obj,slots });
		}


		void upate_ui_data()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("my gui");
			ImGui::Text("ui components ");
			//ImGui::ShowDemoWindow();

			invoke_slots();

			ImGui::End();

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

		}

		template<typename window_t>
		void init(window_t* window)
		{
			// Setup Dear ImGui context
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

			// Setup Platform/Renderer bindings
			const char* glsl_version = "#version 130";
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

		}

	private:
		void invoke_slots()
		{
			std::ranges::for_each(m_connects,
				[](auto& it)
				{
					std::ranges::for_each(it.second, [](auto slot) { slot(); });
				}
			);
		}

	private:

		std::unordered_map<std::shared_ptr<ui_component_base>, std::vector<std::function<void()>>> m_connects;

	};


}
