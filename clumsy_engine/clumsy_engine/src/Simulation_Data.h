

#pragma once

#include <vector>

namespace clumsy_engine
{
	class Simulation_Data
	{

	public:

		void set_mesh(const std::vector<float>& positions, const std::vector<int>& triangles)
		{
			m_positions = positions;
			m_triangles = triangles;

			m_last_frame_positions = m_positions;

			int position_size = positions.size();

			int vertex_num = get_vertex_num();

			m_velocities.resize(position_size);

			m_masses = std::vector<float>(vertex_num, 1.0f);
		}

		int get_vertex_num() const
		{
			return m_positions.size() / 3;
		}



	public:

//		std::vector<float> m_gravity;
//
//		float m_time_step;

		std::vector<float> m_masses;

		std::vector<float> m_velocities;

		std::vector<float> m_positions;

		std::vector<float> m_last_frame_positions;

		std::vector<int> m_triangles;
	};

}