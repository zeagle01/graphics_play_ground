


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

		friend bool operator==(const Element_Equation& l, const Element_Equation& r)
		{
			return l.A == r.A
				&& l.b == r.b
				&& l.stencil == r.stencil;
		}
		static Element_Equation with_size(int vNum)
		{
			Element_Equation ret;
			ret.A.resize(vNum * vNum);
			ret.b.resize(vNum * 3);
			ret.stencil.resize(vNum);
			return ret;
		}
	};

	class System_Equations_Solver
	{
	public:

		virtual void solve(std::vector<float>& x, std::vector<Element_Equation> const equations);

	};


}