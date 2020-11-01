
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
	Simulator_Test()
	{
		m_sim.add_interaction<Inertial>();

		m_sim.set<data::Time_Step>(0.1);

	}

protected:

	std::vector<float>  setup_and_update()
	{

		m_sim.update();

		return m_sim.get<data::Delta_Position>();

	}


	std::vector<float> m_positions;

	std::vector<int> m_triangles;

	int m_vertex_num;

	Simulator m_sim;

};





/////////////////////////////////////
class One_Vertex :public Simulator_Test
{
public:
	One_Vertex()
	{
		m_positions = std::vector<float>(3, 0.f);

		m_vertex_num = m_positions.size() / 3;

		m_sim.set<data::Position>(m_positions);

	}

};


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
	m_sim.add_interaction<Gravity>();

	m_sim.set<data::Gravity>({ 0,-10,0 });

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

		m_sim.add_interaction<Spring_Stretch>();

		m_sim.set<data::Position>(m_positions);

		m_sim.set<data::Triangle_Indices>(m_triangles);
	}

};


TEST_F(One_Edge, update_with_one_edge)
{

	setup_and_update();

	auto act = m_sim.get_edge_lengths();

	std::vector<float> exp = { 1 };

	EXPECT_THAT(act, Eq(exp));

}


