
#pragma once

#include "type_map.h"

namespace soft_render
{
	class Spinning_Cube;


	class Spinning_Cube_App
	{
	public:
		void run();

	public:
		struct config
		{
			ADD_MEMBER_POINTER(model, Spinning_Cube);
		};
		type_map m_configs;
	};

}