
module;

#include <vector>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <limits>

export module  app:simulation_layer;

import sim_lib;

class simulation_layer
{
public:
	using float3 = std::array<float, 3>;
	void init_sim(
		std::vector<int>& m_preset_fix_points,
		std::vector<float3>& m_preset_fix_points_pos,
		std::vector<int>& m_current_fix_points,
		std::vector<float3>& m_current_fix_points_pos,
		const std::vector< int>& indices,
		const std::vector< int>& m_edges,
		const std::vector<float>& pos,
		const std::vector<float>& pos_2d,
		const std::array<int, 2>& m_plane_resolution,
		bool m_enable_edge_stretch_constraint,
		bool m_enable_face_stretch_constraint,
		float uniform_edge_stretch_stiff,
		sim_lib::triangle_stretch_stiff uniform_triangle_stretch_stiff
	)
	{
		sim.init();

		init_sim_data(
		 m_preset_fix_points,
		 m_preset_fix_points_pos,
		 m_current_fix_points,
		 m_current_fix_points_pos,
		 indices,
		 m_edges,
		 pos,
		 pos_2d,
		 m_plane_resolution,
		m_enable_edge_stretch_constraint,
		m_enable_face_stretch_constraint,
		uniform_edge_stretch_stiff,
		 uniform_triangle_stretch_stiff
		);

		sim.commit_all_changes();
	}

	void init_sim_data(
		std::vector<int>& m_preset_fix_points,
		std::vector<float3>& m_preset_fix_points_pos,
		std::vector<int>& m_current_fix_points,
		std::vector<float3>& m_current_fix_points_pos,
		const std::vector< int>& indices,
		const std::vector< int>& m_edges,
		const std::vector<float>& pos,
		const std::vector<float>& pos_2d,
		const std::array<int, 2>& m_plane_resolution,
		bool m_enable_edge_stretch_constraint,
		bool m_enable_face_stretch_constraint,
		float uniform_edge_stretch_stiff,
		sim_lib::triangle_stretch_stiff uniform_triangle_stretch_stiff
	)
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

		if (m_enable_edge_stretch_constraint)
		{
			std::vector<sim_lib::int2> sim_edges;
			convert_to_sim_data(sim_edges, m_edges);
			sim.set<sim_lib::sim_data::stretch_edges>(sim_edges);
			sim.set<sim_lib::sim_data::stretch_edges_stiff>(std::vector<float>(sim_edges.size(), uniform_edge_stretch_stiff));
		}

		if (m_enable_face_stretch_constraint)
		{
			std::vector<int> stretch_t(sim_tris.size());
			std::iota(stretch_t.begin(), stretch_t.end(), 0);
			sim.set<sim_lib::sim_data::stretch_triangles>(stretch_t);
			sim.set<sim_lib::sim_data::stretch_triangles_stiff>(std::vector<sim_lib::triangle_stretch_stiff>(stretch_t.size(), uniform_triangle_stretch_stiff));
		}
	}


	void connect_sim_ui()
	{
	}

private:
	template<typename T, int N>
	static void convert_to_sim_data(std::vector<std::array<T, N>>& out, const std::vector<T>& in)
	{
		int eNum = in.size() / N;
		out.resize(eNum);

		for (int i = 0; i < eNum; i++)
		{
			for (int j = 0; j < N; j++)
			{
				out[i][j] = in[i * N + j];
			}
		}
	}

private:
	sim_lib::simulator sim;
};
