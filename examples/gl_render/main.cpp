
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glog/logging.h"
#include "gflags/gflags.h"
#include <filesystem>



#include <algorithm>

#include "renderer.h"


#include "vertex_buffer.h" 
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"

#include "shader.h"
#include "texture.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



int main(int argc, char** argv)
{

	std::filesystem::create_directory("opengl_render_log");
	FLAGS_log_dir = "opengl_render_log";//out put dir
	FLAGS_alsologtostderr = 1;//also print to console

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);


	auto status = glfwInit();
	if (status == GLFW_TRUE)
	{
		LOG(INFO) << "glfw init succeed! ";
	}
	else
	{
		LOG(ERROR) << "glfw init failed! ";
	}


//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "gl window", nullptr, nullptr);

	if (window)
	{
		LOG(INFO) << "window " << (void*)window << " created";
	}
	else
	{
		LOG(ERROR) << "window create failed!" ;
	}

	glfwMakeContextCurrent(window);
	LOG(INFO) << "gl context " ;

	glfwSwapInterval(1);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		LOG(ERROR)<<"glad load failed";
	}
	else {
		LOG(INFO)<<"glad load succeed!";
	}

	LOG(INFO) << "gl version " << glGetString(GL_VERSION);






	std::vector<float> position
	{
		-0.5f,-0.5f,0.0f,0.0f,
		0.5f,-0.5f,1.0f,0.0f,

		0.5f,0.5f,1.0f,1.0f,
		-0.5f,0.5f,0.0f,1.0f
	};


	std::vector<unsigned int> indices
	{
		0,1,2,
		2,3,0
	};

	std::vector<float> uniform_color{ 1,0,0,1 };

	GL_Call(glEnable(GL_BLEND));
	GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Shader shader;
	shader.create_shader_from_file("cases/gl_render.vs", "cases/gl_render.fs");
	shader.bind();

	Vertex_Buffer vbo(position.data(),position.size());
	Index_Buffer ibo(indices.data(), indices.size());


	Vertex_Array va;
	Vertex_Buffer_Layout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va.add_buffer(vbo, layout);
	
	Texture texture("resources/textures/awesomeface.png");
	int active_texture_slot = 0;
	texture.bind(active_texture_slot);
	shader.set_uniform_1i("u_texture", active_texture_slot);

	Renderer renderer;

	// Setup Dear ImGui context
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 130";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	int frame = 0;


	glm::mat4 proj = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0, 0));
	glm::vec3 model_translate = glm::vec3(0.2f, 0.2f, 0);


	while (!glfwWindowShouldClose(window))
	{

		renderer.clear();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float f = 0.1;
		float a = 0.02;
		for (size_t i = 0; i < position.size() / 3; i++)
		{
			position[i * 2 + 0] += a * std::sin(frame * f);
		}
		indices[0] = frame % 2;

		for (size_t i = 0; i < uniform_color.size(); i++)
		{
			uniform_color[i] = (std::sin(frame * f + i) + 1.) * 0.5;
		}


		glm::mat4 model = glm::translate(glm::mat4(1.0f), model_translate);
		glm::mat4 mvp = proj * view * model;

		shader.set_uniform_4f("u_color", uniform_color[0], uniform_color[1], uniform_color[2], uniform_color[3]);
		shader.set_uniform_mat4f("u_MVP", &mvp[0][0]);


		va.bind();
		vbo.bind();
		vbo.set_data(position.data(), position.size());
		ibo.set_data(indices.data(), indices.size());


		renderer.draw(va, ibo, shader);

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGui::SliderFloat3("float", &model_translate.x, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}


		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GL_Call(glfwSwapBuffers(window));
		GL_Call(glfwPollEvents());






		frame++;
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;

}