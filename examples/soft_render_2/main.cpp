
#include <iostream> 


#include "glad/glad.h"
#include "GLFW/glfw3.h"

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

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0., 0.3, 0.5, 1.);//default color;
		glClear(GL_COLOR_BUFFER_BIT);

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
