
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

	void ui_slider_bar_float3::operator()()
	{
		ImGui::SliderFloat3(name.c_str(), &(*value)(0), min, max);
	}


	void Imgui_Wrapper::init(GLFWwindow* window)
	{
		// Setup Dear ImGui context
		ImGui::CreateContext();

		// Setup Platform/Renderer bindings
		const char* glsl_version = "#version 130";
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

	}



	void Imgui_Wrapper::update()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Test from demo layer");
		ImGui::Text("hello world from demo layer");
		//ImGui::ShowDemoWindow();

		for_each_type<extract_member_type_list_t<gui_component>>::template apply<call>(m_ui_components);

		ImGui::End();

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

}

