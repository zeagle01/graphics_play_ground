
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

	Shader shader;
	shader.create_shader_from_file("cases/gl_render.vs", "cases/gl_render.fs");
	shader.bind();

	std::vector<float> position
	{
		-0.5f,-0.5f,
		0.5f,-0.5f,

		0.5f,0.5f,
		-0.5f,0.5f,
	};


	std::vector<unsigned int> indices
	{
		0,1,2,
		2,3,0
	};



	Vertex_Buffer vbo(position.data(),position.size());
	Index_Buffer ibo(indices.data(), indices.size());


	Vertex_Array va;
	Vertex_Buffer_Layout layout;
	layout.push<float>(2);
	va.add_buffer(vbo, layout);
	
	std::vector<float> uniform_color{ 1,0,0,1 };

	int frame = 0;
	while (!glfwWindowShouldClose(window))
	{

		GL_Call(glClear(GL_COLOR_BUFFER_BIT));

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

		shader.set_uniform_4f("u_color", uniform_color[0], uniform_color[1], uniform_color[2], uniform_color[3]);

		va.bind();
		vbo.bind();

		vbo.set_data(position.data(), position.size());


		ibo.set_data(indices.data(), indices.size());

		GL_Call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		GL_Call(glfwSwapBuffers(window));
		GL_Call(glfwPollEvents());
		frame++;
	}

	glfwTerminate();

	return 0;

}