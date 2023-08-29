
#include "app.h"

#include <chrono>
#include <sstream>



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
			if (m_sim_data_is_valid)
			{
				sim.step();
				convert_from_sim_data(pos, sim.get<sim_lib::sim_data::positions>());
			}
		};

	//m.register_frame_update_fn(animation_fn);

	auto render_fn = [&]
		{
			renderer.draw_triangles(indices.data(), pos.data(), indices.size() / 3, pos.size() / 3);
		};

	auto update_fn = [&]
		{
			fps_fn(animation_fn,m_sim_fps)();
			fps_fn(render_fn,m_render_fps)();
		};

	// update fn
	m.register_frame_update_fn(fps_fn(update_fn,m_fps));
	//m.register_frame_update_fn(update_fn);

	update_sim_data();

	m.run_event_loop();

}

void App::make_plane(float lx, float ly, int nx, int ny)
{

	float dx = lx / nx;
	float dy = ly / ny;

	pos.clear();
	pos.reserve(nx * ny);

	for (int xi = 0; xi < nx; xi++)
	{
		for (int yi = 0; yi < ny; yi++)
		{
			pos.push_back(xi * dx);
			pos.push_back(yi * dy);
			pos.push_back(0);
		}
	}
	pos.shrink_to_fit();

	indices.clear();
	indices.reserve((nx - 1) * (ny - 1) * 2);

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
		}
	}
	indices.shrink_to_fit();
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

	std::vector<sim_lib::int3> sim_tris;
	convert_to_sim_data(sim_tris, indices);

	sim.set<sim_lib::sim_data::solver>(sim_lib::solver_type::Dummy);
	sim.set<sim_lib::sim_data::vertex_num>(sim_pos.size());
	sim.set<sim_lib::sim_data::positions>(sim_pos);
	sim.set<sim_lib::sim_data::positions>(sim_pos);
	sim.set<sim_lib::sim_data::triangles>(sim_tris);
	sim.set<sim_lib::sim_data::obstacle_vert_index>(fix_points);

}

void App::init_sim()
{
	sim.init();

	init_sim_data();

	m_sim_data_is_valid = sim.commit_all_changes();
}

void App::update_sim_data()
{

	m.add_ui_component<ui_component::text_line>("draping plane");

	m.add_ui_component<ui_component::slider_bar2>("plane_size", m_plane_size, { 0.1,1.5 },
		[this](const auto& new_v)
		{
			make_plane(new_v[0], new_v[1], m_plane_resolution[0], m_plane_resolution[1]);
			init_sim_data();
			m_sim_data_is_valid = sim.commit_all_changes();
		}
	);

	m.add_ui_component<ui_component::slider_bar2i>("plane_resolution", m_plane_resolution, { 2,200 },
		[this](const auto& new_v)
		{
			make_plane(m_plane_size[0], m_plane_size[1], new_v[0], new_v[1]);
			init_sim_data();
			m_sim_data_is_valid = sim.commit_all_changes();
		}

	);

	m.add_ui_component<ui_component::slider_bar3>("gravity", sim.get<sim_lib::sim_data::gravity>(), {0.0,10.f},
		[this](const auto& new_v)
		{
			sim.mark_changed<sim_lib::sim_data::gravity>();
			m_sim_data_is_valid = sim.commit_all_changes();
		}
	);

}