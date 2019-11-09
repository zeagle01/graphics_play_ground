





#include "Scene.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "boost/filesystem.hpp"
#include <memory>


int main(int argc,char** argv){


	//std::string currentPath = boost::filesystem::system_complete(".").string();

	boost::filesystem::create_directory("log");
	FLAGS_log_dir = "./log/";

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);


    std::shared_ptr<Scene>s=Scene::getSingleton();
    s->main_loop(argc,argv);
    return 0;

}
