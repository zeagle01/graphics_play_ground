
#include "gmock/gmock.h"

#include "interaction.h"
#include "simulation_interactions.h"
#include "gravity.h"
#include "inertial.h"
#include "spring_stretch.h"
#include "Simulator.h"

#include <memory>

using namespace testing;
using namespace clumsy_engine;

using namespace matrix_math;



class Simulator_Test :public Test
{
public:
	Simulator_Test()
	{
		m_sim.add_interaction<interaction::Inertial>();

		m_sim.set_value<data::Time_Step>(0.1);

		//m_sim.set<data::Mass_Density>(1.);


	}

protected:

	std::vector<vec3f> m_positions;

	Simulator m_sim;

};





/////////////////////////////////////
class One_Vertex :public Simulator_Test
{
public:
	One_Vertex()
	{
		m_positions = std::vector<vec3f>{ get_uniform<3,1,float>(0) };

		m_sim.set_value<data::Position>(m_positions);

		m_sim.set_value<data::Mass>({ 1.f });

	}

};


TEST_F(One_Vertex, without_any_interaction_just_floating_there)
{

	m_sim.set_value<data::Velocity>({ { 0,0,0 } });

	 m_sim.update();

	 auto act = m_sim.get_value<data::Delta_Position>();

	std::vector<vec3f> exp = {
		{0,0.0,0}
	};

	for (int i = 0; i < exp.size(); i++)
	{
		EXPECT_TRUE(is_near<2>(act[i], exp[i]));
	}
}



TEST_F(One_Vertex, update_with_gravity_only)
{
	m_sim.add_interaction<interaction::Gravity>();

	m_sim.set_value<data::Gravity_Acceleration>({ 0,-10,0 });
	m_sim.set_value<data::Velocity>({ { 0,0,0 } });

	m_sim.update();
	
	auto act = m_sim.get_value<data::Delta_Position>();

	std::vector<vec3f> exp = {
		{0,-0.1,0}
	};

	EXPECT_TRUE(is_near<0>(act[0], exp[0], 1e-4f));
}

//////////////////////////////////////////
class One_Edge :public One_Vertex
{
public:
	One_Edge()
	{
		m_positions = {
			{0, 0,0},
			{1, 0,0}
			};

		m_sim.add_interaction<interaction::Spring_Stretch>();

		m_sim.set_value<data::Position>(m_positions);

		m_sim.set_value<data::Mass>({ 1.f,1.f });

		m_sim.set_value<data::Edge_Indice>({ 0,1 });
	}

};


TEST_F(One_Edge, update_with_one_edge)
{

//	m_sim.update();
//	m_sim.update();
//	m_sim.update();
//	m_sim.update();

//	auto act = m_sim.get<data::Edge_Length>();
//
//	std::vector<float> exp = { 1 };
//
//	EXPECT_THAT(act[0], FloatNear(exp[0],1e-2f));

}


