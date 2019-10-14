

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "glog/logging.h"
#include "gflags/gflags.h"
#include "boost/filesystem.hpp"

int main(int argc,char** argv){


	//std::string currentPath = boost::filesystem::system_complete(".").string();

	boost::filesystem::create_directory("log");
	FLAGS_log_dir = "./log/";

	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);

    ::testing::InitGoogleMock(&argc,argv);



    return  RUN_ALL_TESTS();


}
