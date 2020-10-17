

#include  "gmock/gmock.h"
#include <vector>


#include "system_equations_solver.h"


using namespace testing;
using namespace clumsy_engine;



class System_Equations_Solver_Test :public Test
{
protected:
	System_Equations_Solver m_solver;
};



class With_Only_Diagnal_Term :public System_Equations_Solver_Test
{

public:

	std::vector<float> test_diagonal_equations(std::vector<float> A, std::vector<float> b)
	{
		std::vector<float> ret(b.size());
		m_equations.resize(A.size());
		for (int i = 0; i < A.size(); i++)
		{
			m_equations[i].A = std::vector<float>{ A[i] };

			m_equations[i].b = std::vector<float>{ b[i * 3 + 0],b[i * 3 + 1],b[i * 3 + 2] };

			m_equations[i].stencil = std::vector<int>{ i };
		}

		m_solver.solve(ret, m_equations);
		return  ret;
	}


protected:
	std::vector<Element_Equation> m_equations;

};


TEST_F(With_Only_Diagnal_Term, equation_with_only_inertial_term)
{
	std::vector<float> A
	{
		1,2,1
	};
	std::vector<float> b
	{
		1,0,0,
		2,0,0,
		3,0,0
	};

	std::vector<float> exp =
	{
		1,0,0,
		1,0,0,
		3,0,0
	};

	auto act = test_diagonal_equations(A, b);

	EXPECT_THAT(act, Eq(exp));
}


