

#pragma once

#include "mat.h"
#include "type_map.h"
#include "event.h"

namespace soft_render 
{
	class Shader;
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

		bool on_mouse_pressed(const Mouse_Pressed& e);
		bool on_mouse_released(const Mouse_Released& e);
		bool on_mouse_scroll(const Mouse_Scrolled_Event& e);

	private:

		mat4 get_translate_matrix(const vec3& translate);

		mat4 get_model_matrix(const vec3& translate);

		mat4 get_camara_matrix();

		mat4 get_scale_and_translate(const std::array<vec3, 2>& box_from, const std::array<vec3, 2>& box_dst);

		mat4 get_projection_matrix();

		mat4 view_port_matrix();

		void compute_view_projection_matrix();

	private:
		void draw_line(const std::array<vec3, 2>& x, const vec3& color, const mat4& model_matrix);
		void draw_triangles(const std::vector<vec3>& positions, const std::vector<vec3i>& indices, const std::vector<vec3>& normals, const vec3& color, const mat4& model_matrix);
		void draw_cubic(const vec3& corner, float side_length, const vec3& color, const mat4& model_matrix);

	private:

		mat4 m_vp;
		mat4 m_view_matrix;
		mat4 m_projection_matrix;

		int m_width;
		int m_height;
		float m_aspect;

		Drawing_Buffer* m_screen;
		std::shared_ptr<Shader> m_shader;

	public:
		struct config
		{
			ADD_MEMBER_POINTER(angle_rate, vec3, vec3{ 0.1f,0.2f,0.03f });
			ADD_MEMBER_POINTER(init_angle, vec3);
			ADD_MEMBER_POINTER(camara, Camara);
			ADD_MEMBER_POINTER(light_pos, vec3, vec3{ 0,0,100.f });
			ADD_MEMBER_POINTER(light_color, vec3, vec3{ 1.f,1.f,1.f });
			ADD_MEMBER_POINTER(ambient, float, 0.3f);
			ADD_MEMBER_POINTER(cube_side, float, 50.f);
			ADD_MEMBER_POINTER(cube_unit, float, 1.f);
			ADD_MEMBER_POINTER(perpective, bool, false);
			ADD_MEMBER_POINTER(draw_axis, bool, true); 
			ADD_MEMBER_POINTER(draw_light, bool, true); 
		};
		type_map m_configs;
	};

}

