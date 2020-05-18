
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

#include "test_clear_color.h"


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







	GL_Call(glEnable(GL_BLEND));
	GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	Renderer renderer;

	// Setup Dear ImGui context
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	const char* glsl_version = "#version 130";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	int frame = 0;

	test::Test_Clear_Color test;

	while (!glfwWindowShouldClose(window))
	{

		renderer.clear();

		test.on_update(0.0f);
		test.on_render();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		test.on_imgui_render();

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