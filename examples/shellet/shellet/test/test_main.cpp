

#include "gmock/gmock.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include <filesystem>

int main(int argc,char** argv){


	//std::string currentPath = boost::filesystem::system_complete(".").string();

	std::filesystem::create_directory("log");
	FLAGS_log_dir = "./log/";

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);

    ::testing::InitGoogleMock(&argc,argv);



    return  RUN_ALL_TESTS();


}
