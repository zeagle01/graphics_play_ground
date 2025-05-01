
module;

#include <algorithm>
#include <numeric>
#include <cmath>
#include <array>

export module quick_shell : camera;

import matrix_math;

namespace quick_shell
{
	export
	class camera
	{
	public:
		camera()
		{
			m_camara_location = matrix_math::vec3f({ 0.f,0.f,1.f });
			m_look_at = matrix_math::vec3f({ 0.f,0.f,0.f });
			m_camera_up = matrix_math::vec3f({ 0.f,1.f,0.f });
		}


		void set_position(matrix_math::vec3f pos)
		{
			m_camara_location += 1e-3f * pos;
		}

		void set_look_at(matrix_math::vec3f pos)
		{
			m_look_at = pos;
		}

		void set_up_direction(matrix_math::vec3f dir)
		{
			m_camera_up = dir;
		}

		void set_view_field(float near, float far, float fov)
		{
			m_near = near;
			m_far = far;
			m_fov = fov;
		}

		void set_is_perspective(bool v)
		{
			m_is_perspective = v;
		}

		matrix_math::mat4f get_view_projection_matrix()
		{
			using namespace matrix_math;

			mat4f view_matrix = get_view_matrix(m_camara_location, m_look_at, m_camera_up);

			float n = std::abs(m_near);
			float f = std::abs(m_far);

			mat4f projection_matrix
			(
				{
					n,			0,			0,				0,
					0,			n,			0,				0,
					0,			0,			n + f,			n * f,
					0,			0,			-1.f,			0
				}
			);

			mat4f  view_port_matrix = get_view_port_matrix();

			if (m_is_perspective)
			{
				return view_port_matrix * projection_matrix * view_matrix;
			}
			else
			{
				return view_port_matrix *  view_matrix;
			}
		}

	private:

		matrix_math::mat4f get_view_matrix(const matrix_math::vec3f& camera_location, const matrix_math::vec3f& look_at, const matrix_math::vec3f& up_direction)
		{

			matrix_math::vec3f z = normalize(camera_location - look_at);
			matrix_math::vec3f x = normalize(cross(up_direction, z));
			matrix_math::vec3f y = normalize(cross(z, x));

			matrix_math::mat4f frame_matrix
			(
				{
					x(0),y(0),z(0),	0.f,
					x(1),y(1),z(1),	0.f,
					x(2),y(2),z(2),	0.f,
					0	,0	 ,0	  ,1
				}
			);

			matrix_math::mat4f translate_matrix
			(
				{
					1.f,	0.f,	0.f,	-m_camara_location(0),
					0.f,	1.f,	0.f,	-m_camara_location(1),
					0.f,	0.f,	1.f,	-m_camara_location(2),
					0.f	,	0.f,	0.f,	1
				}
			);

			return frame_matrix * translate_matrix;
		}

		matrix_math::mat4f get_view_port_matrix()
		{
			using namespace matrix_math;

			std::array<vec3f, 2> screen_space_box
			{
				vec3f({-1.f,			-1.f,		-1.f}),
				vec3f({1.f,				1.f ,		1.f})
			};


			float tangent = std::tan(degree_2_radian(m_fov));
			float half_height = std::abs(m_near) * tangent;
			float half_width = half_height * m_aspect;

			std::array<vec3f, 2> object_space_box
			{
				vec3f({ -half_width  , -half_height  , m_near}),
				vec3f({  half_width  ,  half_height  , m_far })
			};

			return get_box_transform_matrix(object_space_box, screen_space_box);
		}

	private:
		float degree_2_radian(float d)
		{
			const float pi = 3.1415926f;
			return d * pi / 180.f;
		}

		static matrix_math::mat4f get_box_transform_matrix(const std::array<matrix_math::vec3f, 2>& box_from, const std::array<matrix_math::vec3f, 2>& box_dst)
		{
			using namespace matrix_math;
			mat4f translate_back_to_origin = identity<float, 4>();

			translate_back_to_origin(0, 3) = -0.5f * (box_from[0](0) + box_from[1](0));
			translate_back_to_origin(1, 3) = -0.5f * (box_from[0](1) + box_from[1](1));
			translate_back_to_origin(2, 3) = -0.5f * (box_from[0](2) + box_from[1](2));

			vec3f dx_from = box_from[1] - box_from[0];


			mat4f translate_to_dst = identity<float, 4>();

			translate_to_dst(0, 3) = 0.5f * (box_dst[0](0) + box_dst[1](0));
			translate_to_dst(1, 3) = 0.5f * (box_dst[0](1) + box_dst[1](1));
			translate_to_dst(2, 3) = 0.5f * (box_dst[0](2) + box_dst[1](2));

			mat4f scale_to_dst = identity<float, 4>();
			vec3f dx_dst = box_dst[1] - box_dst[0];

			scale_to_dst(0, 0) = dx_dst(0) / dx_from(0);
			scale_to_dst(1, 1) = dx_dst(1) / dx_from(1);
			scale_to_dst(2, 2) = dx_dst(2) / dx_from(2);

			//return  translate_to_dst * scale_to_dst * translate_back_to_origin;

			return   scale_to_dst ;
		}

	private:
		matrix_math::vec3f m_camara_location;
		matrix_math::vec3f m_look_at;
		matrix_math::vec3f m_camera_up;
		float m_near = -1.f;
		float m_far = -1e1f;
		float m_fov = 45.f; //degree

		float m_aspect = 1.f;

		bool m_is_perspective = false;
	private:

	};



	export
	class camera_controller
	{
	public:
		void reset()
		{
			m_init = false;
		}

		matrix_math::vec2f update_dx(int dx, int dy)
		{
			m_curr_pos = matrix_math::vec2f({ float(dx),float(dy) });

			matrix_math::vec2f  ret{};

			if (m_init)
			{
				ret = m_curr_pos - m_last_pos;
			}

			m_last_pos = m_curr_pos;

			m_init = true;
			return ret;
		}

	private:
		bool m_init = false;
		matrix_math::vec2f m_last_pos;
		matrix_math::vec2f m_curr_pos;
	};


	//class d
}