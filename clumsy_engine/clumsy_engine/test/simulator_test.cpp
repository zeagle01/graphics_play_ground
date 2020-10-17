
#include "gmock/gmock.h"

#include "interaction.h"
#include "gravity.h"
#include "Simulator.h"

#include <memory>

using namespace testing;
using namespace clumsy_engine;


class One_Triangle :public Test
{
public:
	One_Triangle()
	{
		X = std::vector<float>(9, 0.f);

		m_sim.set_mesh(X, T);
	}
protected:

	std::vector<float> X;
	std::vector<int> T;

	Simulator m_sim;

};




TEST_F(One_Triangle, update_with_gravity_only)
{
	float time_stpe = 0.1f;
	std::vector<float> masses(3, 1.f);

	int vertex_num = X.size()/3;

	m_sim.add_interaction(std::make_unique<Gravity>(vertex_num, 0, -10.f, 0, time_stpe, masses));

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







