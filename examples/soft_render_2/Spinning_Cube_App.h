
#pragma once

#include "type_map.h"
#include "mat.h"
#include <memory>
#include <iostream>

namespace soft_render
{
	class Spinning_Cube;
	class Drawing_Buffer;
	class Imgui_Wrapper;
	class GLFW_Wrapper;

	struct rectangle_size { int width; int height; };
	static std::ostream& operator<<(std::ostream& out, const rectangle_size& r)
	{
		out << r.width << " " << r.height;
		return out;
	}
	static std::istream& operator>>(std::istream& in, rectangle_size& r)
	{
		in >> r.width >> r.height;
		return in;
	}


	class Spinning_Cube_App
	{
	public:

		void init();
		void run();

	public:

		struct config
		{
			ADD_MEMBER_POINTER(model, Spinning_Cube);
			ADD_MEMBER_POINTER(screen_size, vec2, vec2{ 800,600 }); //width height
		};
		type_map m_configs;

	private:
		std::shared_ptr<Drawing_Buffer> m_drawing_buffer;
		std::shared_ptr<Imgui_Wrapper> m_gui;
		std::shared_ptr<GLFW_Wrapper> m_glfw_win;

	};

}
