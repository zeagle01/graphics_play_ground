
import quick_shell;
import sim_lib;

#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>

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

int main()
{

	quick_shell::main_window m;
	m.init(800,600);

	auto& renderer = m.get_renderer();

	std::vector< int> indices = { 0,1,2 };
	std::vector<float> pos = {
		0.f,0.f,0.f,
		1.f,0.f,0.f,
		1.f,1.f,0.f
	};
	
	auto render_fn = [&]
		{
			renderer.draw_triangles(indices.data(), pos.data(), 1, 3);
		};


	sim_lib::simulator sim;

	std::vector<sim_lib::float3> sim_pos;
	convert_to_sim_data(sim_pos, pos);

	std::vector<sim_lib::int3> sim_tris;
	convert_to_sim_data(sim_tris, indices);

	sim.init();
	sim.set<sim_lib::sim_data::positions>(sim_pos);
	sim.set<sim_lib::sim_data::triangles>(sim_tris);

	auto animation_fn = [&]()
		{
			sim.step();
			convert_from_sim_data(pos, sim.get<sim_lib::sim_data::positions>());
		};



	// update fn
	m.register_frame_update_fn(render_fn);
	m.register_frame_update_fn(animation_fn);

	m.run_event_loop();

	return 0;

}