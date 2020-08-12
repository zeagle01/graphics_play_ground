


#include "imgui_layer.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include "application.h"
#include "window.h"
#include "log.h"
#include "event.h"
#include "mouse_event.h"
#include "application_event.h"
#include "key_event.h"
#include "handy_helpers.h"

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

			Dispatcher dispatcher(e);

			dispatcher.dispatch<Mouse_Button_Pressed_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::on_mouse_button_pressed_event));
			dispatcher.dispatch<Mouse_Button_Released_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::on_mouse_button_release_event));
			dispatcher.dispatch<Mouse_Moved_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::on_mouse_moved_event));
			dispatcher.dispatch<Mouse_Scrolled_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::on_mouse_scrolled_event));
			dispatcher.dispatch<Key_Pressed_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::on_key_pressed_event));
			dispatcher.dispatch<Key_Typed_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::on_key_typed_event));
			dispatcher.dispatch<Key_Release_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::on_key_released_event));
			dispatcher.dispatch<Window_Resize_Event>(CE_BIND_MEMBER_FN(Imgui_Layer::window_resize_event));

		}


		/////////////EVENTS/////////////

		bool Imgui_Layer::on_mouse_button_pressed_event(Mouse_Button_Pressed_Event& e)
		{
			CE_CORE_INFO("im gui handle {0}", e.get_name());
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[e.get_mouse_button()] = true;
			return false;

		}
		bool Imgui_Layer::on_mouse_button_release_event(Mouse_Button_Released_Event& e)
		{

			CE_CORE_INFO("im gui handle {0}", e.get_name());
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[e.get_mouse_button()] = false;
			return false;
		}
		bool Imgui_Layer::on_mouse_moved_event(Mouse_Moved_Event& e)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos = ImVec2(e.get_x(), e.get_y());

			return false;
		}
		bool Imgui_Layer::on_mouse_scrolled_event(Mouse_Scrolled_Event& e)
		{

			ImGuiIO& io = ImGui::GetIO();
			io.MouseWheelH += e.get_x_offset();
			io.MouseWheel += e.get_y_offset();

			return false;
		}
		bool Imgui_Layer::on_key_pressed_event(Key_Pressed_Event& e)
		{
			CE_CORE_INFO("im gui handle {0}", e.get_name());
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[e.get_key()] = true;

			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];


			return false;
		}
		bool Imgui_Layer::on_key_released_event(Key_Release_Event& e)
		{
			CE_CORE_INFO("im gui handle {0}", e.get_name());
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[e.get_key()] = false;
			return false;

		}

		bool Imgui_Layer::on_key_typed_event(Key_Typed_Event& e)
		{
			ImGuiIO& io = ImGui::GetIO();

			int c = e.get_key();

			if (c > 0 && c < 0x10000)
			{
				io.AddInputCharacter((unsigned short)c);
			}

			return false;
		}

		bool Imgui_Layer::window_resize_event(Window_Resize_Event& e)
		{

			CE_CORE_INFO("im gui handle {0}", e.get_name());

			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(e.get_width(), e.get_height());	
			io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
			glViewport(0, 0, e.get_width(), e.get_height());

			return false;
		}

}