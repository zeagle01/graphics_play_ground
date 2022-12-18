

#pragma once

#include "mat.h"
#include "type_map.h"

namespace soft_render 
{
	class Drawing_Buffer;


	struct config
	{
		ADD_MEMBER_POINTER(angle_rate, vec3, vec3{ 0.1f,0.2f,0.03f });
		ADD_MEMBER_POINTER(init_angle, vec3);
		ADD_MEMBER_POINTER(lookat, vec3, vec3{ 50,50,0 });
		ADD_MEMBER_POINTER(camara_location, vec3, vec3{ 600,600,100 });
		ADD_MEMBER_POINTER(cube_side, float, 100.f);
		ADD_MEMBER_POINTER(cube_unit, float, 1.2f);
	};


	class Spinning_Cube
	{
	public:
		Spinning_Cube(int w, int h, Drawing_Buffer* sc);

		void update();

		template<typename name>
		auto& get_config()
		{
			return m_datas.get_ref<name>();
		}

	private:

		mat4 get_rotation_matrix();

		mat4 get_camara_matrix();

		mat4 get_scale_and_translate(const std::array<vec3, 2>& box_from, const std::array<vec3, 2>& box_dst);

		mat4 to_pixel_space();
		void compute_pixel(float x, float y, float z);

	private:

		int m_width;
		int m_height;
		Drawing_Buffer* screen;
		float m_aspect = float(m_width) / float(m_height);
		float m_world_height = 400.f;
		vec3 m_up{ 0,1,0 };

		type_map m_datas;
	};

}

