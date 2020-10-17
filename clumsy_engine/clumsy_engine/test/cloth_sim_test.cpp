
#include "gmock/gmock.h"

using namespace testing;


class Cloth_Sim
{

	std::vector<float> m_delta_positions = {
			0,-0.1,0,
			0,-0.1,0,
			0,-0.1,0
	};

	std::vector<float> m_edge_rest_length = {
		1.f,
		1.f,
		1.f
	};

public:
	void set_gravity(std::vector<float> gravity_acc) {}
	void set_time_step(float h) {}
	void set_edge_rest_length(const std::vector<float>& edge_rest_length) 
	{

	};

	void update() {}

	void update_to_converge() 
	{
		m_edge_rest_length = {
		2.f,
		2.f,
		2.f
		};
	}

	const std::vector<float>& get_delta_positions()const
	{
		return m_delta_positions;
	}
	const std::vector<float>& get_edge_length()const {
		return m_edge_rest_length;
	};


private:


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






TEST_F(One_Triangle, test_edge_rest_length)
{
	auto act = m_sim.get_edge_length();

	std::vector<float> exp = {
		1.f,
		1.f,
		1.f
	};

	EXPECT_THAT(act, exp);
}









////////////////////////// stretch test
class One_Triangle_With_Stretch_Setup: public One_Triangle
{
public:

protected:
	virtual void SetUp() override
	{
		m_sim.set_gravity({ 0,0,0 });
		m_sim.set_time_step(0.1f);

	}

	auto set_rest_length_and_run(const std::vector<float>& rest_length)
	{
		m_sim.set_edge_rest_length({ 2.f, 2.f,2.f });
		m_sim.update_to_converge();

		return m_sim.get_edge_length();
	}

};


TEST_F(One_Triangle_With_Stretch_Setup, stretch_to_greater_rest_length)
{

	std::vector<float> exp = {
		2.f,
		2.f,
		2.f
	};

	auto act = set_rest_length_and_run(exp);

	EXPECT_THAT(act, Eq(exp));

}


TEST_F(One_Triangle_With_Stretch_Setup, stretch_to_less_rest_length)
{
	std::vector<float> exp = {
		0.5f,
		0.5f,
		0.5f
	};

	auto act = set_rest_length_and_run(exp);

	EXPECT_THAT(act, Eq(exp));

}

////////////////////////// stretch test



