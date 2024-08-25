
#include "app.h"

#include <chrono>
#include <sstream>
#include <numeric>
#include <limits>

using namespace quick_shell;

template<typename T,int N>
static void convert_to_sim_data(std::vector<std::array<T, N>>& out, const std::vector<T>& in)
{
	int eNum = in.size()/N;
	out.resize(eNum);

	for (int i = 0; i < eNum; i++)
	{
		for (int j = 0; j < N; j++)
		{
			out[i][j] = in[i * N + j];
		}
	}
}

template<typename T,int N>
static void convert_from_sim_data(std::vector<T>& out, const std::vector<std::array<T, N>>& in)
{
	int eNum = in.size();
	out.resize(eNum * N);

	for (int i = 0; i < eNum; i++)
	{
		for (int j = 0; j < N; j++)
		{
			out[i * N + j] = in[i][j];
		}
	}
}


void App::run()
{
	m.init(800, 600);

	auto& renderer = m.get_renderer();

	prepare_mesh();

	m_sim_data_is_str = m_sim_data_is_valid ? "yes" : "no";
	m.add_ui_component<ui_component::text_line>("sim_data_is_ready", m_sim_data_is_str);
	m.add_ui_component<ui_component::text_line>("fps", m_fps);
	m.add_ui_component<ui_component::text_line>("sim_fps", m_sim_fps);
	m.add_ui_component<ui_component::text_line>("render_fps", m_render_fps);


	init_sim();

	auto fps_fn = [&](auto fn, std::string& fps)
		{
			return [&,fn] {
				auto b = std::chrono::high_resolution_clock::now();

				fn();

				auto e = std::chrono::high_resolution_clock::now();
				std::stringstream ss;

				ss << 1e9 / (e - b).count();

				fps = ss.str();
			};
		};

	auto animation_fn = [&]()
		{
			m_sim_data_is_valid = sim.step();

			if (m_sim_data_is_valid)
			{
				convert_from_sim_data(pos, sim.get<sim_lib::sim_data::positions>());
			}
		};

	//m.register_frame_update_fn(animation_fn);

	auto render_fn = [&]
		{
			renderer.draw_triangles(indices.data(), pos.data(), indices.size() / 3, pos.size() / 3);

			if (m_picked.t_index != -1)
			{
				int t = m_picked.t_index;
				renderer.draw_points(m_mouse_ray.p.data(), 1);
			}

			renderer.draw_points(m_current_fix_points_pos.front().data(), m_current_fix_points.size());
		};

	auto update_fn = [&]
		{
			fps_fn(animation_fn,m_sim_fps)();
			fps_fn(render_fn,m_render_fps)();
		};

	// update fn
	m.register_frame_update_fn(fps_fn(update_fn,m_fps));

	//event fn
	m.register_event_fn<mouse_pressed>([this](const auto& e) 
		{
			pick(e.x, e.y);
			return true;
		});

	m.register_event_fn<mouse_moved>([this](const auto& e) 
		{
			drag(e.x, e.y);
			return true;
		});

	m.register_event_fn<mouse_released>([this](const auto& e) 
		{
			release_pick();
			return true;
		});



	connect_sim_ui();
	connect_render_ui();

	m.run_event_loop();

}

void App::pick(int x, int y)
{
	m_mouse_ray = {
		{ x / 400.f-1.0f ,1.0f - y / 300.f ,-1.f },
		{ 0 , 0, 1.f }
	};

	m_picked = m_mouse_picker.pick(
		m_mouse_ray.p,
		m_mouse_ray.dir,
		pos.data(), indices.data(), pos.size() / 3, indices.size() / 3 
	);

	m_current_fix_points = m_preset_fix_points;
	m_current_fix_points_pos = m_preset_fix_points_pos;
	if (m_picked.t_index != -1)
	{
		int t = m_picked.t_index;
		std::array<int, 3> t_indices{ indices[t * 3 + 0] ,indices[t * 3 + 1],indices[t * 3 + 2] };

		float maxBary = 0.f;
		int max_v = 0;
		for (int i = 0; i < 3; i++)
		{
			if (m_picked.bary[i] > maxBary)
			{
				maxBary = m_picked.bary[i];
				max_v = i;
			}
		}

		int picked_v = t_indices[max_v];
		m_current_fix_points.push_back(picked_v);
		m_current_fix_points_pos.push_back(float3{ pos[picked_v * 3 + 0],pos[picked_v * 3 + 1],pos[picked_v * 3 + 2] });

		sim.set<sim_lib::sim_data::obstacle_vert_index>(m_current_fix_points);
		sim.set<sim_lib::sim_data::obstacle_vert_pos>(m_current_fix_points_pos);
		sim.commit_all_changes();
	}
}

void App::drag(int x, int y)
{
	if (m_picked.t_index != -1)
	{

		int t = m_picked.t_index;

		std::array<int, 3> t_indices{ indices[t * 3 + 0] ,indices[t * 3 + 1],indices[t * 3 + 2] };
		float maxBary = 0.f;
		int max_v = 0;
		for (int i = 0; i < 3; i++)
		{
			if (m_picked.bary[i] > maxBary)
			{
				maxBary = m_picked.bary[i];
				max_v = i;
			}
		}

		std::vector<int> v_indices{ t_indices[max_v] };
		std::vector<sim_lib::float3> v_new_pos{ {x / 400.f - 1.0f ,1.0f - y / 300.f ,-1.f} };

		std::vector<int> v_indices_pos;
		for (int i = 0; i < m_current_fix_points.size(); i++)
		{
			if (m_current_fix_points[i] == v_indices[0])
			{
				v_indices_pos.push_back(i);
				m_current_fix_points_pos[i] = v_new_pos[0];
				break;
			}
		}

		sim.set_partially<sim_lib::sim_data::obstacle_vert_pos>(v_indices_pos, v_new_pos);
		sim.commit_all_changes();
	}
}

void App::release_pick()
{
	m_picked.t_index = -1;
	m_current_fix_points = m_preset_fix_points;
	m_current_fix_points_pos = m_preset_fix_points_pos;

	sim.set<sim_lib::sim_data::obstacle_vert_index>(m_current_fix_points);
	sim.set<sim_lib::sim_data::obstacle_vert_pos>(m_current_fix_points_pos);
	sim.commit_all_changes();
}

void App::make_plane(float lx, float ly, int nx, int ny)
{

	float dx = lx / nx;
	float dy = ly / ny;

	pos.clear();
	pos.reserve(nx * ny);
	pos_2d.clear();
	pos_2d.reserve(nx * ny);

	for (int xi = 0; xi < nx; xi++)
	{
		for (int yi = 0; yi < ny; yi++)
		{
			pos.push_back(xi * dx);
			pos.push_back(yi * dy);
			pos.push_back(0);

			pos_2d.push_back(xi * dx);
			pos_2d.push_back(yi * dy);
		}
	}
	pos.shrink_to_fit();
	pos_2d.shrink_to_fit();

	indices.clear();
	indices.reserve((nx - 1) * (ny - 1) * 2);
	m_edges.clear();
	m_edges.reserve(nx * ny * 3);

	for (int xi = 0; xi < nx - 1; xi++)
	{
		for (int yi = 0; yi < ny - 1; yi++)
		{
			int v0 = yi + ny * xi;
			int v1 = yi + 1 + ny * xi;

			int v2 = yi + ny * (xi + 1);
			int v3 = yi + 1 + ny * (xi + 1);
			indices.push_back(v0);
			indices.push_back(v1);
			indices.push_back(v2);

			indices.push_back(v2);
			indices.push_back(v1);
			indices.push_back(v3);

			//edges
			m_edges.push_back(v0);
			m_edges.push_back(v1);

			m_edges.push_back(v0);
			m_edges.push_back(v2);

			m_edges.push_back(v1);
			m_edges.push_back(v2);

			m_edges.push_back(v2);
			m_edges.push_back(v3);

			m_edges.push_back(v1);
			m_edges.push_back(v3);
		}
	}
	indices.shrink_to_fit();
	m_edges.shrink_to_fit();
}

void App::prepare_mesh()
{

	 indices = { 0,1,2 };
	 pos = {
		0.f,0.f,0.f,
		1.f,0.f,0.f,
		1.f,1.f,0.f
	};

	 make_plane(m_plane_size[0], m_plane_size[1], m_plane_resolution[0], m_plane_resolution[1]);

}

void App::init_sim_data()
{

	std::vector<sim_lib::float3> sim_pos;
	convert_to_sim_data(sim_pos, pos);
	std::vector<sim_lib::float2> sim_pos_2d;
	convert_to_sim_data(sim_pos_2d, pos_2d);

	std::vector<sim_lib::int3> sim_tris;
	convert_to_sim_data(sim_tris, indices);

	sim.set<sim_lib::sim_data::solver>(sim_lib::solver_type::Dummy);
	sim.set<sim_lib::sim_data::vertex_num>(sim_pos.size());
	sim.set<sim_lib::sim_data::positions>(sim_pos);
	sim.set<sim_lib::sim_data::rest_positions>(sim_pos);
	sim.set<sim_lib::sim_data::material_positions>(sim_pos_2d);
	sim.set<sim_lib::sim_data::triangles>(sim_tris);


	int fix_v0 = 0 * m_plane_resolution[1] + m_plane_resolution[1] - 1;
	int fix_v1 = (m_plane_resolution[0] - 1) * m_plane_resolution[1] + m_plane_resolution[1] - 1;

	m_preset_fix_points = { fix_v0, fix_v1 };
	m_preset_fix_points_pos = { sim_pos[fix_v0], sim_pos[fix_v1] };

	m_current_fix_points = m_preset_fix_points;
	m_current_fix_points_pos = m_preset_fix_points_pos;

	sim.set<sim_lib::sim_data::obstacle_vert_index>(m_preset_fix_points);
	sim.set<sim_lib::sim_data::obstacle_vert_pos>(m_preset_fix_points_pos);

	std::vector<sim_lib::int2> sim_edges;
	convert_to_sim_data(sim_edges, m_edges);
	sim.set<sim_lib::sim_data::stretch_edges>(sim_edges);
	sim.set<sim_lib::sim_data::stretch_edges_stiff>(std::vector<float>(sim_edges.size(), uniform_edge_stretch_stiff));

	std::vector<int> stretch_t(sim_tris.size());
	std::iota(stretch_t.begin(), stretch_t.end(),0);
	sim.set<sim_lib::sim_data::stretch_triangles>(stretch_t);
	sim.set<sim_lib::sim_data::stretch_triangles_stiff>(
		std::vector<sim_lib::triangle_stretch_stiff>(stretch_t.size(), uniform_triangle_stretch_stiff)
	);

}

void App::init_sim()
{
	sim.init();

	init_sim_data();

	sim.commit_all_changes();
}

void App::connect_sim_ui()
{

	m.add_ui_component<ui_component::text_line>("draping plane");

	m.add_ui_component<ui_component::slider_bar2>("plane_size", m_plane_size, { 0.1,1.5 },
		[this](const auto& new_v)
		{
			make_plane(new_v[0], new_v[1], m_plane_resolution[0], m_plane_resolution[1]);
			init_sim_data();
			sim.commit_all_changes();
		}
	);

	m.add_ui_component<ui_component::slider_bar2i>("plane_resolution", m_plane_resolution, { 2,200 },
		[this](const auto& new_v)
		{
			make_plane(m_plane_size[0], m_plane_size[1], new_v[0], new_v[1]);
			init_sim_data();
			sim.commit_all_changes();
		}

	);

	m.add_ui_component<ui_component::text_line>("simulator");

	m.add_ui_component<ui_component::slider_bar3>("gravity", sim.get<sim_lib::sim_data::gravity>(), {0.0,10.f},
		[this](const auto& new_v)
		{
			sim.mark_changed<sim_lib::sim_data::gravity>();
			sim.commit_all_changes();
		}
	);

	m.add_ui_component<ui_component::input_float>("time_step", sim.get<sim_lib::sim_data::time_step>(), {0.0001,1.f},
		[this](const auto& new_v)
		{
			sim.mark_changed<sim_lib::sim_data::time_step>();
			sim.commit_all_changes();
		}
	);

	m.add_ui_component<ui_component::input_float>("density", sim.get<sim_lib::sim_data::density>(), {0.0001,1.f},
		[this](const auto& new_v)
		{
			sim.mark_changed<sim_lib::sim_data::density>();
			sim.commit_all_changes();
		}
	);

	m.add_ui_component<ui_component::input_float>("stetch_edge_stiff", uniform_edge_stretch_stiff, {0.000,1e6f},
		[this](const auto& new_v)
		{
			sim.set<sim_lib::sim_data::stretch_edges_stiff>(std::vector<float>(m_edges.size()/2, new_v));

			sim_lib::triangle_stretch_stiff new_trinagle_stiff = { new_v,new_v,new_v,new_v };

			sim.set<sim_lib::sim_data::stretch_triangles_stiff>(
				std::vector<sim_lib::triangle_stretch_stiff>(indices.size() / 3, new_trinagle_stiff)
			);

			sim.commit_all_changes();
		}
	);

//	m.add_ui_component<ui_component::slider_bar>("stretch stiff", sim.get<sim_lib::sim_data::stretch_stiff>(), {0.0001,1.f},
//		[this](const auto& new_v)
//		{
//			sim.mark_changed<sim_lib::sim_data::stretch_stiff>();
//			m_sim_data_is_valid = sim.commit_all_changes();
//		}
//	);

}

void App::connect_render_ui()
{

	m.add_ui_component<ui_component::text_line>("render configs ");

	auto& renderer = m.get_renderer();

	m.add_ui_component<ui_component::check_box>("enable_fill", m_enable_fill, {},
		[&,this](const auto& new_v)
		{
			renderer.enable_fill(new_v);
		}
	);

	m.add_ui_component<ui_component::check_box>("enable_wireframe", m_enable_wirefram, {},
		[&,this](const auto& new_v)
		{
			renderer.enable_wireframe(new_v);
		}
	);


}
