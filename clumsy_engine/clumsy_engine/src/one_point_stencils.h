
#pragma once

#include "interaction.h"
#include <vector>

namespace clumsy_engine
{


	class One_Point_Stencils
	{

	public:
		std::vector<stencil> operator()(int n);
		
	private:
		std::vector<stencil> m_stencils;

	};

}