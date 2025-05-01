
module;

#include <map>

export module app;

import :components;
import :plane_maker;

import quick_shell;
import clumsy_lib;

export
class app
{
public:
	void run()
	{
		m_main_window.init(800, 600);

		init();

		m_main_window.register_frame_update_fn([this](int x, int y) { update(x, y); });

		m_main_window.run_event_loop();

	}

private:

	void init()
	{
		make_plane();
		m_camera_controller.reset();
	}

	void update(int x, int y)
	{
		auto dx = m_camera_controller.update_dx(x, y);

		m_camera.set_position(matrix_math::vec3f({ dx(0),dx(1),0.f }));

		render();

	}

private:
	void make_plane()
	{
		plane_maker pm;
		auto id = clumsy_lib::id_generator::new_id();
		position_buffer pos;
		triangle_index_buffer triangles;
		pm.create_plane(pos, triangles, 0.5f, 0.5f, 5, 10);
		m_position_buffer[id] = pos;
		m_triangle_buffer[id] = triangles;
	}

	void  render()
	{
		auto& renderer = m_main_window.get_renderer();

		for (auto [id, pos] : m_position_buffer)
		{
			auto triangles = m_triangle_buffer[id];

			const vec3f* posPtr = pos.data.get();
			const vec3i* indexPtr = triangles.data.get();

			matrix_math::mat4f vp = m_camera.get_view_projection_matrix();
			//matrix_math::mat4f vp = matrix_math::identity<float, 4>();

			renderer.draw_triangles(&(indexPtr[0](0)), &(posPtr[0](0)), triangles.num, pos.num, vp);
		}

	}

private:
	quick_shell::main_window m_main_window;
	quick_shell::camera_controller m_camera_controller;
	quick_shell::camera m_camera;

private:

	std::map<clumsy_lib::id, position_buffer> m_position_buffer;
	std::map<clumsy_lib::id, triangle_index_buffer> m_triangle_buffer;
};
