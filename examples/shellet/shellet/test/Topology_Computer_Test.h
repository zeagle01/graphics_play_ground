

#ifndef TOPOLOGY_COMPUTER_TEST_H
#define TOPOLOGY_COMPUTER_TEST_H


#include <memory>

#include "gtest/gtest.h"

#include "Topology_Computer.h"




class Topology_Computer_Test :public ::testing::Test{

public:
	Topology_Computer_Test() :tc(std::make_shared<Topology_Computer>()) {}


protected:
	std::shared_ptr<Topology_Computer> tc;

};


#endif
