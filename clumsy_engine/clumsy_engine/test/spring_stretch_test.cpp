



#include "gmock/gmock.h"

#include "spring_stretch.h"

using namespace testing;
using namespace clumsy_engine;

class Spring_Stretch_Test :public Test
{
public:
	Spring_Stretch_Test()
	{
		m_sim_data = std::make_shared<Simulation_Data>();
	}

	std::shared_ptr<Simulation_Data> m_sim_data;
};


TEST_F(Spring_Stretch_Test, one_spring)
{
	Spring_Stretch spring_stretch;
	std::vector<float > pos;
	std::vector<int> triangles;
	auto stencils = spring_stretch.compute_stencils(pos, triangles);

	std::vector<stencil> exp{ {0,1} };

	EXPECT_THAT(stencils, Eq(exp));

}