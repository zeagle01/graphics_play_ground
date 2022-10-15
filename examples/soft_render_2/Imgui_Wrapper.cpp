
#include "Imgui_Wrapper.h"

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
		ImGui::End();

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
