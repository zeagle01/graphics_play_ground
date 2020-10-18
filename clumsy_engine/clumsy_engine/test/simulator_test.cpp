
#include "gmock/gmock.h"

#include "interaction.h"
#include "gravity.h"
#include "Simulator.h"

#include <memory>

using namespace testing;
using namespace clumsy_engine;


class One_Vertex :public Test
{
public:
	One_Vertex()
	{
		m_positions = std::vector<float>(3, 0.f);

		m_vertex_num = m_positions.size() / 3;

		m_sim.set_mesh(m_positions, m_triangles);
	}
protected:

	std::vector<float>  setup_and_update()
	{
		m_sim.set_time_step(0.1f);

		m_sim.update();

		return m_sim.get_delta_positions();

	}


	std::vector<float> m_positions;

	std::vector<int> m_triangles;

	int m_vertex_num;

	Simulator m_sim;

};




TEST_F(One_Vertex, update_with_gravity_only)
{

	float time_stpe = 0.1f;
	std::vector<float> masses(m_vertex_num, 1.f);

	m_sim.add_interaction(std::make_unique<Gravity>(m_vertex_num, 0, -10.f, 0, time_stpe, masses));

	auto act = setup_and_update();

	std::vector<float> exp = {
		0,-0.1,0
	};

	EXPECT_THAT(act, exp);
}

TEST_F(One_Vertex, without_any_interaction_just_floating_there)
{

	auto act = setup_and_update();

	std::vector<float> exp = {
		0,0.0,0
	};

	EXPECT_THAT(act, exp);
}







