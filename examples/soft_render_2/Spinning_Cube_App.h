
#pragma once

#include "type_map.h"
#include <memory>

namespace soft_render
{
	class Spinning_Cube;
	class Drawing_Buffer;
	class Imgui_Wrapper;


	class Spinning_Cube_App
	{
	public:

		void init();
		void run();

	public:
		struct config
		{
			ADD_MEMBER_POINTER(model, Spinning_Cube);
		};
		type_map m_configs;

	private:
		void set_connection();

		std::shared_ptr<Drawing_Buffer> m_drawing_buffer;
		std::shared_ptr<Imgui_Wrapper> m_gui;

		int m_width = 800;
		int m_height = 600;
	};

}