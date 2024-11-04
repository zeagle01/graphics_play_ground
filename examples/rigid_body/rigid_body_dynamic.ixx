
module;

#include <vector>
#include <array>
#include <string>

export module rigid_body_dynamic;

import :helper;


import quick_shell;
import matrix_math;

using vec3f = matrix_math::matrix<float, 3, 1>;
using vec2f = matrix_math::matrix<float, 2, 1>;
using vec3i = matrix_math::matrix<int, 3, 1>;
using vec2i = matrix_math::matrix<int, 2, 1>;

using mat3f = matrix_math::matrix<float, 3, 3>;

export
class rigid_body_dynamic
{
public:
	void run() 
	{

		m_window.init(800, 600);

		auto& renderer = m_window.get_renderer();

		m_center = vec3f({ 0,0,0 });
		m_velocity = vec3f({ 0.f,0,0 });
		m_angular_moment = vec3f({ 1.f,0,1.f });
		m_orientation = matrix_math::identity<float, 3>();

		std::array<float, 3> ui_velocity = vec_2_arr(m_velocity);
		std::array<float, 3> ui_angular_moment = vec_2_arr(m_angular_moment);
		std::array<float, 3> ui_center = vec_2_arr(m_center);
		std::array<float, 3> ui_side_length = vec_2_arr(m_side_length);


		m_window.register_frame_update_fn(
			[&]()
			{
				m_window.add_ui_component<quick_shell::ui_component::slider_bar3>("rect side length", ui_side_length, { 0.1,2.f });
				m_window.add_ui_component<quick_shell::ui_component::slider_bar3>("linear velocity", ui_velocity, { 0.0,5.f });
				m_window.add_ui_component<quick_shell::ui_component::slider_bar3>("angular moment", ui_angular_moment, { 0.0,100.f });

				m_velocity =				arr_2_vec(ui_velocity);
				m_angular_moment =			arr_2_vec(ui_angular_moment);

				m_side_length =				arr_2_vec(ui_side_length);
				m_rotationInertial0 = get_cuboid_rotation_inertial(m_side_length);

				auto angular_velocity = m_orientation / m_rotationInertial0 * matrix_math::transpose(m_orientation) * m_angular_moment;

				m_center += m_velocity * dt;
				m_orientation += schew_matrix(angular_velocity) * m_orientation * dt;

				auto rigid_body_mesh = get_repsentation_mesh(m_orientation, m_center);

				renderer.draw_triangles(as_buffer(rigid_body_mesh.triangles), as_buffer(rigid_body_mesh.positions), rigid_body_mesh.triangles.size(), rigid_body_mesh.positions.size());
			}
		);

		m_window.run_event_loop();
	}

private:
	struct mesh
	{
		std::vector<vec3f> positions;
		std::vector<vec3i> triangles;
	};

private:

	mesh get_repsentation_mesh(const mat3f& orientation, const vec3f& center)
	{
		mesh l_mesh = generate_unit_cube();

		transplate_to_cubic_center(l_mesh);

		auto& x = l_mesh.positions;

		scale(x, m_side_length);

		rotate(x, m_orientation);

		translate(x, m_center);

		return l_mesh;
	}

	void transplate_to_cubic_center(mesh& p_mesh)
	{
		translate(p_mesh.positions, vec3f({ -0.5f,-0.5f,-0.5f }));
	}

	void scale(std::vector<vec3f>& x, const vec3f& s)
	{
		for (int i = 0; i < x.size(); i++)
		{
			x[i](0) *= s(0);
			x[i](1) *= s(1);
			x[i](2) *= s(2);
		}
	}

	void translate(std::vector<vec3f>& x, const vec3f& dx)
	{
		for (int i = 0; i < x.size(); i++)
		{
			x[i] += dx;
		}
	}

	void rotate(std::vector<vec3f>& x, const mat3f& r)
	{
		for (int i = 0; i < x.size(); i++)
		{
			x[i] = r * x[i];
		}
	}

	mesh generate_unit_cube()
	{
		mesh ret;
		auto& x = ret.positions;
		x.reserve(8);

		mat3f identity = matrix_math::identity<float, 3>();
		x.push_back(vec3f({ 0,0,0 }));

		for (int i = 0; i < 3; i++)
		{
			auto old_x = x;
			for (int j = 0; j < old_x.size(); j++)
			{
				vec3f new_x = old_x[j] + matrix_math::column(identity, i);
				x.push_back(new_x);
			}
		}

		std::array<int, 4> face_vertex[] = {
			{0,1,3,2}, //front
			{4,6,7,5}, //back
			{0,4,6,2}, //left
			{1,5,7,3}, //left
			{0,1,5,4}, //bottom
			{2,3,7,6}, //bottom

		};
		int face_num = std::size(face_vertex);
		auto& triangles = ret.triangles;
		triangles.reserve(face_num * 2);
		for (int i = 0; i < std::size(face_vertex); i++)
		{
			const auto& face_i_vertex = face_vertex[i];
			triangles.push_back(vec3i({ face_i_vertex[0], face_i_vertex[1], face_i_vertex[2] }));
			triangles.push_back(vec3i({ face_i_vertex[0], face_i_vertex[2], face_i_vertex[3] }));
		}


		return ret;
	}

	mat3f schew_matrix(const vec3f& v)
	{
		return mat3f({
			0,		-v(2),	v(1),
			v(2),	0,		-v(0),
			-v(1),	v(0),	0
			});
	}

	mat3f get_cuboid_rotation_inertial(const vec3f& side_length)
	{
		float a = side_length(0);
		float b = side_length(1);
		float c = side_length(2);
		float V = a * b * c;

		return 1.f / V * mat3f({
				b*b+c*c,		0,				0,
				0,				a*a+c*c,		0,
				0,				0,				a*a+b*b,
			});
	}


private:
	quick_shell::main_window m_window;

private:
	mat3f m_orientation;
	mat3f m_rotationInertial0;

	vec3f m_center;
	vec3f m_velocity;
	vec3f m_angular_moment;

	vec3f m_side_length = vec3f({ 1.0f,0.5f,1.5f });

	float dt = 1 / 1000.f;

};