
#include "Hello_VK_Triangle.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include <filesystem>
#include <stdexcept>
#include <cstdlib>

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
		return EXIT_FAILURE;
	}




    return EXIT_SUCCESS;
}
