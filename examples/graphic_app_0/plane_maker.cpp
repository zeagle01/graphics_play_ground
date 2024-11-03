
module;

#include "clumsy_lib/enum_tagged_morphysm.h"
#include <memory>
#include <array>
#include <vector>

module app : plane_maker;

import :scene_maker;
import :matrix_using;

import quick_shell;

using namespace quick_shell;

CLUMSY_ENUM_TAGGED_MORPHYSM_IMP_DECLARE(scene_maker_interface, scene_maker_type::make_a_plane)
{
public:
	bool make
	(
		std::vector<vec3i>&indices,
		std::vector<vec2i>&m_edges,
		std::vector<vec3f>&pos,
		std::vector<vec2f>&pos_2d,
		quick_shell::main_window & m
	) override
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

	void on_ui(quick_shell::main_window & m)
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

	void make_plane(
		std::vector<vec3i>&indices,
		std::vector<vec2i>&m_edges,
		std::vector<vec3f>&pos,
		std::vector<vec2f>&pos_2d,
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
				pos.push_back(vec3f({ xi * dx, yi * dy, 0.f }));

				pos_2d.push_back(vec2f({ xi * dx,yi * dy }));
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
				indices.push_back(vec3i({ v0,v1,v2 }));
				indices.push_back(vec3i({ v2,v1,v3 }));

				//edges
				m_edges.push_back(vec2i({v0,v1}));

				m_edges.push_back(vec2i({v0,v2}));

				m_edges.push_back(vec2i({v1,v2}));

				m_edges.push_back(vec2i({v2,v3}));

				m_edges.push_back(vec2i({v1,v3}));
			}
		}
		indices.shrink_to_fit();
		m_edges.shrink_to_fit();
	}

private:
	std::array<float, 2> m_plane_size{ 0.5f,0.5f };
	std::array<int, 2> m_plane_resolution{ 10,10 };
	bool m_changed = true;

};

CLUMSY_ENUM_TAGGED_MORPHYSM_IMP_REGISTER(scene_maker_interface, scene_maker_type::make_a_plane);
