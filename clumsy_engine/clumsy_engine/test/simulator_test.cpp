
#include "gmock/gmock.h"

#include "interaction.h"
#include "gravity.h"
#include "inertial.h"
#include "spring_stretch.h"
#include "Simulator.h"

#include <memory>

using namespace testing;
using namespace clumsy_engine;



class Simulator_Test :public Test
{
public:

protected:

	std::vector<float>  setup_and_update()
	{

		m_sim.add_interaction<Inertial>(0.1f);

		m_sim.update();

		return m_sim.get_delta_positions();

	}


	std::vector<float> m_positions;

	std::vector<int> m_triangles;

	int m_vertex_num;

	Simulator m_sim;

};

class One_Vertex :public Simulator_Test
{
public:
	One_Vertex()
	{
		m_positions = std::vector<float>(3, 0.f);

		m_vertex_num = m_positions.size() / 3;

		m_sim.set_mesh(m_positions, m_triangles);
	}

};


////////////////////////////////////////////////////////////
TEST_F(Simulator_Test, set_then_get_data)
{

	std::vector<float> exp{ 2.f };

	m_sim.set<Position>(exp);

	auto act = m_sim.get<Position>();

	EXPECT_THAT(act, Eq(exp));
}




TEST_F(One_Vertex, without_any_interaction_just_floating_there)
{

	auto act = setup_and_update();

	std::vector<float> exp = {
		0,0.0,0
	};

	EXPECT_THAT(act, exp);
}



TEST_F(One_Vertex, update_with_gravity_only)
{
	m_sim.add_interaction<Gravity>(0, -10.f, 0);

	auto act = setup_and_update();

	std::vector<float> exp = {
		0,-0.1,0
	};

	EXPECT_THAT(act, exp);
}

//////////////////////////////////////////
class One_Edge :public One_Vertex
{
public:
	One_Edge()
	{
		m_positions = {
			0, 0,0,
			1, 0,0
			};

		m_vertex_num = m_positions.size() / 3;

		m_sim.set_mesh(m_positions, m_triangles);
	}

};


TEST_F(One_Edge, update_with_one_edge)
{
	m_sim.add_interaction<Spring_Stretch>();

	m_sim.set_mesh(m_positions, m_triangles);

	setup_and_update();

	auto act = m_sim.get_edge_lengths();

	std::vector<float> exp = { 1 };

	EXPECT_THAT(act, Eq(exp));

}


