
#pragma once

#include "mat.h"
#include "type_map.h"

namespace soft_render
{
	class Camara
	{
	public:
		Camara();
		void on_press();
		void on_release();
		void update(int x, int y);

	private:
		void rotate(float dx, float dy);

	private:
		bool pressed = false;
		bool isCursorAssigned = false;
		int x;
		int y;


		float m_theta = 0.f;
		float m_phi = 0.f;
	public:
		struct config
		{
			ADD_MEMBER_POINTER(lookat, vec3, vec3{ 50,50,0 });
			ADD_MEMBER_POINTER(up, vec3, vec3{ 0,1,0 });
			ADD_MEMBER_POINTER(location, vec3, vec3{ 1000,1000,1000 });
		};
		type_map m_configs;
	};
}
