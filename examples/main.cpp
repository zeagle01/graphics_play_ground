





#include "Scene.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "boost/filesystem.hpp"


int main(int argc,char** argv){

	boost::filesystem::create_directory("log");
	FLAGS_log_dir = "./log/";

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);


    Scene s;
    s.main_loop(argc,argv);
    return 0;

}
