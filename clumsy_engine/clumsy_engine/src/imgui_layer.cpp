


#include "imgui_layer.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include "window.h"
#include "log.h"

namespace clumsy_engine
{

		Imgui_Layer::Imgui_Layer() :Layer("imgui_layer") 
		{
		}
		Imgui_Layer::~Imgui_Layer()
		{

		}

		void Imgui_Layer::on_attach()
		{

			// Setup Dear ImGui context
			ImGui::CreateContext();

			// Setup Platform/Renderer bindings
			//ImGui_ImplGlfw_InitForOpenGL(window, true);

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			ImGuiIO& io = ImGui::GetIO();
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

			io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
			io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
			io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
			io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
			io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
			io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
			io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
			io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
			io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
			io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
			io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
			io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
			io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
			io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
			io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
			io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
			io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
			io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
			io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
			io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
			io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
			io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

			const char* glsl_version = "#version 410";
			ImGui_ImplOpenGL3_Init(glsl_version);
		}

		void Imgui_Layer::on_detach() 
		{
		}

		void Imgui_Layer::on_update() 
		{


			//auto& app = Application::get_singleton();

			ImGuiIO& io=ImGui::GetIO();

			//CE_CORE_INFO("{0} {1}", app.get_window().get_width(), app.get_window().get_height());

			//io.DisplaySize = ImVec2(app.get_window().get_width()/2, app.get_window().get_height()/2);
			io.DisplaySize = ImVec2(800, 600);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			static bool show = true;
			ImGui::ShowDemoWindow(&show);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void Imgui_Layer::on_event(Event& e) 
		{
		}


}