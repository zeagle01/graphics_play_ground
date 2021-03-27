



#include "gmock/gmock.h"

#include "spring_stretch.h"

using namespace testing;
using namespace clumsy_engine;

class Spring_Stretch_Test :public Test
{
public:
	Spring_Stretch_Test()
	{
		m_positions=
		{ 
			{0,0,0},
			{1,0,0 } 
		};
		m_spring_stretch = std::make_shared<Spring_Stretch>();
		m_spring_stretch->set<data::Edge_Indice>({ 0,1 });
		m_spring_stretch->set<data::Stretch_Stiff>(m_stiffness);
		m_spring_stretch->set<data::Edge_Length>({ 1.f });

		m_spring_stretch->set<data::Position>(m_positions);
	}

protected:

	std::vector<float > pos
	{
		0,0,0,
		1,0,0 
	} ;
	std::shared_ptr<Spring_Stretch> m_spring_stretch;
	std::vector<vec3f > m_positions;
	std::vector<int> m_triangles;
	float m_stiffness = 1e1f;
};


TEST_F(Spring_Stretch_Test, one_spring_stencil)
{
	auto stencils = m_spring_stretch->compute_stencils();

	std::vector<stencil> exp{ {0,1} };

	EXPECT_THAT(stencils, Eq(exp));

}

TEST_F(Spring_Stretch_Test, one_spring_equation)
{
	auto equation = m_spring_stretch->compute_element_equation({ 0,1 }, 0);

	auto id = get_identity<3, float>();

	Element_Equation exp{
		{
			m_stiffness*id,-m_stiffness*id,
			-m_stiffness*id,m_stiffness*id
		},
		{
			{-m_stiffness,0,0},
			{m_stiffness,0,0}
		},
		{0,1}
	};

	EXPECT_THAT(equation, Eq(exp));

}
