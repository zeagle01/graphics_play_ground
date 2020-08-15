





#include "Scene.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include <filesystem>


int main(int argc,char** argv){


	//std::string currentPath = boost::filesystem::system_complete(".").string();

	std::filesystem::create_directory("log");
	FLAGS_log_dir = "./log/";//out put dir
	FLAGS_alsologtostderr = 1;//also print to console

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);


    Scene s;
    s.main_loop(argc,argv);
    return 0;

}
