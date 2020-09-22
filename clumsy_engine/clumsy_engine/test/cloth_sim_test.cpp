
#include "gmock/gmock.h"

using namespace testing;


class Cloth_Sim
{

	std::vector<float> m_delta_positions = {
			0,-0.1,0,
			0,-0.1,0,
			0,-0.1,0
	};

public:
	void set_gravity(std::vector<float> gravity_acc) {}
	void set_time_step(float h) {}

	void update() {}

	const std::vector<float>& get_delta_positions()const
	{
		return m_delta_positions;
	}

};



class One_Triangle :public Test
{

protected:

	std::vector<float> X;
	std::vector<int> T;

	Cloth_Sim m_sim;

};




TEST_F(One_Triangle, update_with_gravity_only)
{
	m_sim.set_gravity({ 0,-10.f,0 });
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


