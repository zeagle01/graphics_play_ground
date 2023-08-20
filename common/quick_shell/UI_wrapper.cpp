module;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <functional>
#include <unordered_map>
#include <memory> 
#include <ranges>
#include <algorithm>


module quick_shell : UI_wrapper;

import : ui_components;
import : ui_component_imp;

namespace quick_shell
{

	class ui_panel
	{
	public:
		template<typename  ui_com>
		void add_ui_component(const std::string& name, typename ui_com::type& value, const typename ui_com::extra_data& extra_d)
		{
			ui_component_imp<ui_com> ui_com_obj;
			ui_com_obj.name = name;
			ui_com_obj.value = &value;
			ui_com_obj.extra = extra_d;

			m_connects.push_back(ui_com_obj);
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
			std::ranges::for_each(m_connects, [](auto& slot) { slot(); });
		}

	private:

		 std::vector<std::function<void()>> m_connects;

	};


}
