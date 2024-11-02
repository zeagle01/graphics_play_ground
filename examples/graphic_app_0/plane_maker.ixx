module;

#include <vector>
#include <array>

export module  app:plane_maker;

import matrix_math;
import quick_shell;

using namespace quick_shell;

export
class plane_maker
{
public:
	void make_plane(
		std::vector< int>& indices,
		std::vector< int>& m_edges,
		std::vector<float>& pos,
		std::vector<float>& pos_2d,
		float lx, float ly, int nx, int ny)
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

	bool make_plane(std::vector< int>& indices,
		std::vector< int>& m_edges,
		std::vector<float>& pos,
		std::vector<float>& pos_2d,
		quick_shell::main_window& m)
	{
		on_ui(m);

		bool changed = m_changed;
		if (changed)
		{
			make_plane(indices, m_edges, pos, pos_2d, m_plane_size[0], m_plane_size[1], m_plane_resolution[0], m_plane_resolution[1]);
		}
		m_changed = false;
		return changed;
	}

private:

	void on_ui(quick_shell::main_window& m)
	{
		m.add_ui_component<ui_component::text_line>("draping plane");

		m.add_ui_component<ui_component::slider_bar2>("plane_size", m_plane_size, { 0.1,1.5 },
			[this](const auto& new_v)
			{
				m_changed = true;
			}
		);

		m.add_ui_component<ui_component::slider_bar2i>("plane_resolution", m_plane_resolution, { 2,200 },
			[this](const auto& new_v)
			{
				m_changed = true;
			}

		);
	}

private:
	std::array<float, 2> m_plane_size{ 0.5f,0.5f };
	std::array<int, 2> m_plane_resolution{ 10,10 };
	bool m_changed;
};
