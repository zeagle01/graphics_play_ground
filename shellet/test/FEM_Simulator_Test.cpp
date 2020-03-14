
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


    void compute_new_positions();
};

void FEM_Simulator_Test::SetUp()
{
    m_sim = std::dynamic_pointer_cast<FEM_Simulator>(Simulator::new_instance("FEM"));

    m_x0 = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0,
    };
    m_x = m_x0;
    m_x[2 * 3 + 1] = 2;
}



TEST_F(FEM_Simulator_Test,test_one_triangle)
{
    m_sim->compute_new_stretched_positions(m_x0,m_x);
    EXPECT_THAT(m_x,Eq(m_x0));
}

