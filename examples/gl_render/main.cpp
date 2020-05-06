
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glog/logging.h"
#include "gflags/gflags.h"
#include <filesystem>


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


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		LOG(ERROR)<<"glad load failed";
	}
	else {
		LOG(INFO)<<"glad load succeed!";
	}

	LOG(INFO) << "gl version " << glGetString(GL_VERSION);


	std::vector<float> position
	{
		-0.5f,-0.5f,
		0.0f,0.5f,
		0.5,-0.5f
	};
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER,buffer);

	int position_attrib = 0;
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float),position.data(),GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(position_attrib);

	glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER,0);

	while (!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glDrawArrays(GL_TRIANGLES, 0, position.size() / 2);
		//glDrawElements(GL_TRIANGLES,3,)

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;

}