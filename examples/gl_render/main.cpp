
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glog/logging.h"
#include "gflags/gflags.h"
#include <filesystem>


#include <string>
#include <fstream>
#include <sstream>

#include <algorithm>

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

	auto shader = create_shader(read_source_from_file("cases/gl_render.vs"), read_source_from_file("cases/gl_render.fs"));

	glUseProgram(shader);

	std::vector<float> position
	{
		-0.5f,-0.5f,
		0.5f,-0.5f,

		0.5f,0.5f,
		-0.5f,0.5f,
	};


	std::vector<int> indices
	{
		0,1,2,
		2,3,0
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);

	int position_attrib = glGetAttribLocation(shader,"position");
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(position_attrib);
	glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);

	int frame = 0;
	while (!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);
		float f = 0.1;
		float a = 0.02;
		for (size_t i = 0; i < position.size() / 3; i++)
		{
			position[i * 2 + 0] += a * std::sin(frame * f);
		}

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_DYNAMIC_DRAW);

		indices[0] = frame % 2;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		frame++;
	}

	glDeleteProgram(shader);
	glfwTerminate();

	return 0;

}