
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glog/logging.h"
#include "gflags/gflags.h"
#include <filesystem>


#include <string>
#include <fstream>
#include <sstream>

#include <algorithm>

#include "renderer.h"


#include "vertex_buffer.h" 
#include "index_buffer.h"


static std::string read_source_from_file(const std::string& file)
{
	std::ifstream fin(file);
	if (!fin.good())
	{
		LOG(ERROR) << "faile to open file";
	}

	std::stringstream ss;
	ss << fin.rdbuf();
	return ss.str();
}

static unsigned int compile_shader(std::string& shader,unsigned int type)
{

	unsigned int id = glCreateShader(type);
	const char* src = shader.data();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int lengh;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lengh);
		std::string log_s(lengh,' ');
		glGetShaderInfoLog(id, lengh, &lengh, &log_s[0]);
		auto shader_name=(type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
		LOG(ERROR) << "faile to compile " << shader_name;
		LOG(ERROR) << log_s;
	}

	return id;

}

static auto create_shader(std::string& vertex_shader, std::string& fragment_shader)
{
	auto program = glCreateProgram();
	auto vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	auto fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}





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

	auto shader = create_shader(read_source_from_file("cases/gl_render.vs"), read_source_from_file("cases/gl_render.fs"));

	GL_Call(glUseProgram(shader));

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

	unsigned int vao;
	GL_Call(glGenVertexArrays(1, &vao));
	GL_Call(glBindVertexArray(vao));


	Vertex_Buffer vbo(position.data(),position.size());

	int position_attrib = glGetAttribLocation(shader,"position");
	//GL_Call(glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_DYNAMIC_DRAW));
	GL_Call(glEnableVertexAttribArray(position_attrib));
	GL_Call(glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0));


	Index_Buffer ibo(indices.data(), indices.size());

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

		GL_Call(auto location = glGetUniformLocation(shader, "u_color"));
		glUniform4f(location, uniform_color[0], uniform_color[1], uniform_color[2], uniform_color[3]);

		vbo.set_data(position.data(), position.size());


		ibo.set_data(indices.data(), indices.size());

		GL_Call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		GL_Call(glfwSwapBuffers(window));
		GL_Call(glfwPollEvents());
		frame++;
	}

	GL_Call(glDeleteProgram(shader));
	glfwTerminate();

	return 0;

}