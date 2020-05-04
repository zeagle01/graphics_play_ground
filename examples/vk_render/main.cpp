
#include "Hello_VK_Triangle.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include <filesystem>
#include <stdexcept>

int main(int argc, char** argv)
{
	std::filesystem::create_directory("vulkan_render_log");
	FLAGS_log_dir = "vulkan_render_log";//out put dir
	FLAGS_alsologtostderr = 1;//also print to console

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);

	Hello_VK_Triangle app;

	try
	{
		app.run();
	}
	catch (const std::exception & e)
	{
		LOG(ERROR) << e.what();
	}


//	
//	glfwInit();
//
//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//	GLFWwindow* window = glfwCreateWindow(800, 600, "vulkan window", nullptr, nullptr);
//	uint32_t extension_count;
//	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
//
//	LOG(INFO) << extension_count << " extension_count";
//
//
//	glm::mat4 matrix;
//	glm::vec4 vec;
//	auto test = matrix * vec;
//
//	while (!glfwWindowShouldClose(window))
//	{
//		glfwPollEvents();
//	}


    return 0;
}
