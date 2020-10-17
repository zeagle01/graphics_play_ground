
#include "gmock/gmock.h"

#include "interaction.h"
#include "gravity.h"
#include "Simulator.h"

#include <memory>

using namespace testing;
using namespace clumsy_engine;


class One_Triangle :public Test
{

protected:

	std::vector<float> X;
	std::vector<int> T;

	Simulator m_sim;

};




TEST_F(One_Triangle, update_with_gravity_only)
{
	m_sim.add_interaction(std::make_unique<Gravity>(0, -10.f, 0));

	m_sim.set_time_step(0.1f);

	m_sim.update();


	auto act = m_sim.get_delta_positions();

	std::vector<float> exp = {
		0,-0.1,0,
		0,-0.1,0,
		0,-0.1,0

	};

	EXPECT_THAT(act, exp);
}

TEST_F(One_Triangle, without_any_interaction_just_floating_there)
{
	m_sim.set_time_step(0.1f);

	m_sim.update();

	auto act = m_sim.get_delta_positions();

	std::vector<float> exp = {
		0,0.0,0,
		0,0.0,0,
		0,0.0,0

	};

	EXPECT_THAT(act, exp);
}







