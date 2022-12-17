

#pragma once

#include "mat.h"

class Drawing_Buffer;

struct Spinning_Cube
{
	int m_width;
	int m_height;
	Drawing_Buffer* screen;
	float m_aspect = float(m_width) / float(m_height);
	float cube_side = 100.f;
	float cube_unit = 1.2f;
	float m_world_height = 400.f;

	vec3 m_up{0,1,0};
	vec3 m_lookat{50,50,0};
	vec3 m_camara_location{ 600,600,100 };

	float m_angle_x = 0.f;
	float m_angle_y = 0.f;
	float m_angle_z = 0.f;


	mat4 get_rotation_matrix();
	
	mat4 get_camara_matrix();
	
	mat4 get_scale_and_translate(const std::array<vec3, 2>& box_from, const std::array<vec3, 2>& box_dst);

	mat4 to_pixel_space();
	void compute_pixel(float x, float y, float z);

	void update();


};
