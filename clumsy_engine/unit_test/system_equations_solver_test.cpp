

#include  "gmock/gmock.h"
#include <vector>


#include "system_equations_solver.h"


using namespace testing;
using namespace clumsy_engine;



class System_Equations_Solver_Test :public Test
{
public:
	System_Equations_Solver_Test()
	{
		m_solver = std::make_shared<System_Equations_Solver>();
	}

protected:

	std::shared_ptr<System_Equations_Solver> m_solver;
	std::vector<Element_Equation> m_equations;
	std::vector<vec3f> m_x;
};



class With_Only_Diagnal_Term :public System_Equations_Solver_Test
{

public:
	With_Only_Diagnal_Term()
	{
		Element_Equation eq;
		eq.stencil = std::vector<int>{ 0 };
		eq.A = std::vector<mat3x3f>{ get_identity<3,float>() };
		eq.b = std::vector<vec3f>{ vec3f{ 0,1,0 } };

		auto eq1 = eq;
		eq1.stencil = { 1 };

		m_equations.push_back(eq);
		m_equations.push_back(eq1);
		int vNum = 2;
		m_x.resize(vNum);
	}


};


TEST_F(With_Only_Diagnal_Term, equation_with_only_inertial_term)
{
	m_solver->solve(m_x, m_equations);

	std::vector<vec3f> exp{ {0,1,0} ,{0,1,0} };
	
	EXPECT_THAT(m_x, Eq(exp));
}

class With_One_Spring :public System_Equations_Solver_Test
{

public:
	With_One_Spring()
	{
		Element_Equation eq;
		float stiff = 1e1;
		eq.stencil = std::vector<int>{ 0 ,1 };
		eq.A = std::vector<mat3x3f>{ stiff * get_identity<3,float>(),-stiff * get_identity<3,float>(),-stiff * get_identity<3,float>(),stiff * get_identity<3,float>() };
		//eq.b = std::vector<vec3f>{ vec3f{ 0,1,0 } };
		eq.b = { {-stiff,0,0} ,{stiff,0,0} };
		m_equations.push_back(eq);

		int vNum = 2;
		m_x = { {0,0,0},{1,0,0} };
	}


};



TEST_F(With_One_Spring, equation_with_one_spring)
{
	m_solver->solve(m_x, m_equations);

	std::vector<vec3f> exp{ {0,0,0} ,{1,0,0} };
	
	EXPECT_THAT(m_x, Eq(exp));
}


