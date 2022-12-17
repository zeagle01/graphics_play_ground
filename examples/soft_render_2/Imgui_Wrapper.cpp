
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Imgui_Wrapper.h"

#include <string>
#include <vector>

namespace soft_render
{


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

		//std::vector < std::string> tag{"a", "b", "c"};
		//for (int i = 0; i < m_sliders.size(); i++)
		//{
		//	auto& a = m_sliders[i]();
		//	if (ImGui::SliderFloat(tag[i].c_str(), &((m_sliders[i])()), 0.f, 1.f))
		//	{
		//		printf(" %d %f\n", i, a);
		//	}
		//}

		for (int i = 0; i < m_sliders_float3.size(); i++)
		{
			//ImGui::SliderFloat("aaa", &((m_sliders[i])()), 0.f, 1.f);
			vec3& v = m_sliders_float3[i]();
			ImGui::SliderFloat3("angle_rate", &v(0), 0.f, 1.f);
		}



		ImGui::End();

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void Imgui_Wrapper::add_slider_bar(std::function<float& ()> get_value)
	{
		m_sliders.push_back(get_value);
	}

	void Imgui_Wrapper::add_slider_bar_float3(get_vec3_t get_value)
	{
		m_sliders_float3.push_back(get_value);

	}
}

