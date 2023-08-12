
#pragma once

#include <vector>

class Linear_System_Solver
{
public:
	template<typename matT, typename vecT>
	std::vector<vecT> solve(
		const std::vector<matT>& A,
		const std::vector<int>& I,
		const std::vector<int>& J,
		const std::vector<vecT>& b
	)
	{
		std::vector<vecT> ret;





		return ret;
	}

};
