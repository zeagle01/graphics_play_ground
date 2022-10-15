
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "OpenGL_Wrapper.h"

#include <iostream> 
#include <string>
#include <vector> 



void init_openGL()
{

}

void init_window()
{


	auto status = glfwInit();
	if (status == GLFW_TRUE)
	{
		printf("glfw init succeed! \n");
	}
	else
	{
		printf("glfw init error! \n");
	}

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "gl window", nullptr, nullptr);

	if (window)
	{
		printf(" window %p is created\n", window);
	}
	else
	{
		printf(" window can't be created\n");
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		printf("glad load failed\n");
	}
	else {

		printf("glad load succend\n");
	}
	printf("gl version %s\n", glGetString(GL_VERSION));

	OpenGL_Wrapper gl;

	gl.init();

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0., 0.3, 0.5, 1.);//default color;
		glClear(GL_COLOR_BUFFER_BIT);

		gl.draw();


//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//
//		ImGui::Begin("Test from demo layer");
//		ImGui::Text("hello world from demo layer");
//		//ImGui::ShowDemoWindow();
//		ImGui::End();
//
//		// Rendering
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

}

int main()
{
	printf(" hello world");

	init_window();

	return 0;
}


