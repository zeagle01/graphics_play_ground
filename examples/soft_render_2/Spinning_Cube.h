

#pragma once

#include "mat.h"
#include "type_map.h"

namespace soft_render 
{
	class Drawing_Buffer;
	class Camara;

	class Spinning_Cube
	{
	public:
		void init(int w, int h, Drawing_Buffer* sc);

		void set_spinning_cube_default_value();

		void update(int cx, int cy);

		template<typename name>
		auto& get_config()
		{
			return m_configs.get_ref<name>();
		}

	private:

		mat4 get_translate_matrix(const vec3& translate);

		mat4 get_model_matrix(const vec3& translate);

		mat4 get_camara_matrix();

		mat4 get_scale_and_translate(const std::array<vec3, 2>& box_from, const std::array<vec3, 2>& box_dst);

		mat4 get_projection_matrix();

		mat4 view_port_matrix();

		void compute_view_projection_matrix();

		void compute_pixel(const vec3& pos, const vec3& color, const mat4& model_matrix);

		void draw_cubic(const mat4& model_matrix, float cube_side, float cube_unit);

	private:

		mat4 m_vp;

		int m_width;
		int m_height;
		float m_aspect;

		Drawing_Buffer* m_screen;

	public:
		struct config
		{
			ADD_MEMBER_POINTER(angle_rate, vec3, vec3{ 0.1f,0.2f,0.03f });
			ADD_MEMBER_POINTER(init_angle, vec3);
			ADD_MEMBER_POINTER(camara, Camara);
			ADD_MEMBER_POINTER(cube_side, float, 50.f);
			ADD_MEMBER_POINTER(cube_unit, float, 1.f);

			ADD_MEMBER_POINTER(perpective, bool, false);
		};
		type_map m_configs;
	};

}

