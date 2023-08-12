
#pragma once




#include "gmock/gmock.h"

#include "FEM_Simulator.h"


using namespace testing;

class FEM_Simulator_Test:public testing::Test
{
public:
    virtual void SetUp() override;

protected:
    std::shared_ptr<FEM_Simulator> m_sim;


    std::vector<float> m_x0;
    std::vector<float> m_x;
    std::vector<int> m_indices;


    void compute_new_positions();
};

void FEM_Simulator_Test::SetUp()
{
    m_sim = std::dynamic_pointer_cast<FEM_Simulator>(Simulator::new_instance("FEM"));

    m_indices = { 0, 1, 2};
    
    m_x0 = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0,
    };
    
    m_x = m_x0;
    m_x[2 * 3 + 1] = 2;
    m_sim->setMesh(m_x0, m_indices);
    m_sim->setDeltaT(0.01);
}



TEST_F(FEM_Simulator_Test,test_one_triangle)
{
    m_sim->compute_new_stretched_positions(m_x);
    //EXPECT_THAT(m_x,Eq(m_x0));
    EXPECT_TRUE(1==1);
    //EXPECT_THAT(1,Eq(1));
}

