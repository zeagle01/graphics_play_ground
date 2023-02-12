
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Imgui_Wrapper.h"
#include "type_list.h"
#include "member_extractor.h"

#include <string>
#include <vector>

namespace soft_render
{

	void slider_bar_float3::operator()()
	{
		ImGui::SliderFloat3(name.c_str(), &(*value)(0), extra.min, extra.max);
	}

	void slider_bar_float2::operator()()
	{
		ImGui::SliderFloat2(name.c_str(), &(*value)(0), extra.min, extra.max);
	};

	void slider_bar_float::operator()()
	{
		ImGui::SliderFloat(name.c_str(), value, extra.min, extra.max);
	};

	void check_box::operator()()
	{
		ImGui::Checkbox(name.c_str(), value);
	}


	void Imgui_Wrapper::init(GLFWwindow* window)
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


	struct call_ui_component
	{
		template<typename T>
		static void apply(type_map& tm)
		{
			auto fn = tm.get_ref<T>();
			fn();
		}
	};

	void Imgui_Wrapper::update()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Test from demo layer");
		ImGui::Text("hello world from demo layer");
		//ImGui::ShowDemoWindow();

		for (auto it : m_ui_components)
		{
			it->operator()();
		}

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

}

