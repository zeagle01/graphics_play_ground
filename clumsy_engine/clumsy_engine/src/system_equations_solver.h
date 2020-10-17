


#pragma  once

#include <vector> 

#include <functional>


namespace clumsy_engine
{


	struct Element_Equation
	{
		std::vector<float> A;
		std::vector<float> b;
		std::vector<int> stencil;
	};

	class System_Equations_Solver
	{
	public:

		virtual void solve(std::vector<float>& x, std::vector<Element_Equation> const equations);

	};


}