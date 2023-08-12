


#include "imgui_layer.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include "window.h"
#include "log.h"
#include "event.h"
#include "mouse_event.h"
#include "application_event.h"
#include "key_event.h"
#include "handy_helpers.h"
#include "profiler.h"

namespace clumsy_engine
{

		Imgui_Layer::Imgui_Layer() :Layer("imgui_layer") 
		{
		}
		Imgui_Layer::~Imgui_Layer()
		{

		}

		void Imgui_Layer::begin()
		{
			RECORD_FUNCTION_DURATION();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		
		void Imgui_Layer::end()
		{
			RECORD_FUNCTION_DURATION();

			auto& app = Application::get_singleton();
			auto& window = app.get_window();

			ImGuiIO& io=ImGui::GetIO();
			io.DisplaySize = ImVec2(window.get_width(), window.get_height());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);

			}
		}

		void Imgui_Layer::on_attach()
		{

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			m_imgui_context = ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer bindings

			auto& app = Application::get_singleton();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.get_window().get_window_native());
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}

		void Imgui_Layer::on_detach() 
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

}