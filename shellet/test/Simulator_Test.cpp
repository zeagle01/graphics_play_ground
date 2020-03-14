




#include "gmock/gmock.h"
#include "Simulator.h"
#include "PD_Simulator.h"

//#include <typeinfo>

#include <iostream>

using namespace testing;

class Simulator_Test :public testing::Test
{

};




TEST_F(Simulator_Test,test_create_PD_instance)
{
    auto s_pd =Simulator::new_instance("PD");
    
    auto pd=std::dynamic_pointer_cast<PD_Simulator>(s_pd);
//    std::cout<<typeid(pd).name()<<std::endl;
//    std::cout<<typeid(std::shared_ptr<PD_Simulator>).name()<<std::endl;
    EXPECT_THAT(typeid(pd).name(),Eq(typeid(std::shared_ptr<PD_Simulator>).name()));
}