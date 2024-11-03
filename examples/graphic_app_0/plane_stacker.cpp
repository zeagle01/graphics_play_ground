
module;

#include "clumsy_lib/enum_tagged_morphysm.h"
#include <memory>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

module app:plane_stacker;

import :scene_maker;
import :matrix_using;

import quick_shell;

using namespace quick_shell;

CLUMSY_ENUM_TAGGED_MORPHYSM_IMP_DECLARE(scene_maker_interface, scene_maker_type::stack_planes)
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

			//clear origin data;
			indices.clear();
			m_edges.clear();
			pos.clear();
			pos_2d.clear();

			float height = m_height0;
			for (int i = 0; i < m_layers; i++)
			{
				auto l_mesh = make_plane(m_plane_size[0], m_plane_size[1], m_plane_resolution[0], m_plane_resolution[1], height);
				auto edges = build_edges(l_mesh.triangles);

				stack_mesh(indices, m_edges, pos, pos_2d, l_mesh, edges); //stakc mesh

				height += m_height_step;
			}
			
		}
		m_changed = false;
		return changed;

	}

	std::vector<int> get_fixed()  override
	{ 
		float lx = m_plane_size[0];
		float ly = m_plane_size[1];
		int nx = m_plane_resolution[0];
		int ny = m_plane_resolution[1];

		int vert_per_layer = nx * ny;

		std::vector<int>ret;
		ret.resize(vert_per_layer);
		std::iota(std::begin(ret), std::end(ret), m_layer_that_is_fixed * vert_per_layer);

		return ret;
	};


private:
	struct mesh
	{
		std::vector<vec3i> triangles;
		std::vector<vec3f> pos;
		std::vector<vec2f> pos_2d;
	};


private:
	void on_ui(quick_shell::main_window & m)
	{
		m.add_ui_component<ui_component::text_line>("stack planes");

		auto mark_changed_fn = [this]() { m_changed = true; };

		m.add_ui_component_new<ui_component_new::input>(mark_changed_fn, m_layers, "num of stacked layer");
		m.add_ui_component_new<ui_component_new::input>(mark_changed_fn, m_layer_that_is_fixed, "fixed layer");

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
	mesh make_plane(float lx, float ly, int nx, int ny, float height)
	{
		mesh l_mesh;

		auto& pos = l_mesh.pos;
		auto& pos_2d = l_mesh.pos_2d;
		auto& indices = l_mesh.triangles;

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
				pos.push_back(vec3f({ xi * dx, height, yi * dy }));

				pos_2d.push_back(vec2f({ xi * dx,yi * dy }));
			}
		}
		pos.shrink_to_fit();
		pos_2d.shrink_to_fit();

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
				indices.push_back(vec3i({ v0,v1,v2 }));
				indices.push_back(vec3i({ v2,v1,v3 }));
			}
		}
		indices.shrink_to_fit();

		return l_mesh;
	}

	std::vector<vec2i> build_edges(const std::vector<vec3i>& triangles)
	{
		std::vector<vec2i> ret;
		ret.reserve(triangles.size());

		std::map<int, std::set<int>> vv_checker;

		auto is_edge_exsit = [](const std::map<int, std::set<int>>& checker, int v_min, int v_max)
			{
				if (checker.contains(v_min))
				{
					if (checker.at(v_min).contains(v_max))
					{
						return true;
					}
				}
				return false;
			};


		auto insert_vv_checker = [](std::map<int, std::set<int>> checker, int v_min, int v_max)
			{
				if (!checker.contains(v_min))
				{
					checker[v_min] = {};
				}

				checker[v_min].insert(v_max);

			};

		for (int i = 0; i < triangles.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int v0 = triangles[i](j);
				int v1 = triangles[i]((j + 1) % 3);

				int v_min = std::min(v0, v1);
				int v_max = std::max(v0, v1);

				if (is_edge_exsit(vv_checker, v_min, v_max))
				{
					ret.push_back(vec2i({ v_min,v_max }));

					insert_vv_checker(vv_checker, v_min, v_max);
				}
			}

		}
		ret.shrink_to_fit();
		return ret;
	}


	void stack_mesh(
		std::vector<vec3i>& indices,
		std::vector<vec2i>& m_edges,
		std::vector<vec3f>& pos,
		std::vector<vec2f>& pos_2d,
		const mesh& p_mesh,
		const std::vector<vec2i>& edges
	)
	{
		int v_offset = pos.size();

		int indices_begin = indices.size();
		indices.resize(indices.size() + p_mesh.triangles.size());
		for (int i = 0; i < p_mesh.triangles.size(); i++)
		{
			indices[indices_begin + i] = p_mesh.triangles[i] + v_offset;
		}

		int edge_begin = m_edges.size();
		m_edges.resize(m_edges.size() + edges.size());
		for (int i = 0; i < edges.size(); i++)
		{
			m_edges[edge_begin + i] = edges[i] + v_offset;
		}

		pos.insert(pos.end(), p_mesh.pos.begin(), p_mesh.pos.end());
		pos_2d.insert(pos_2d.end(), p_mesh.pos_2d.begin(), p_mesh.pos_2d.end());
	}

private:
	std::array<float, 2> m_plane_size{ 0.5f,0.5f };
	std::array<int, 2> m_plane_resolution{ 10,10 };
	bool m_changed = true;

	int m_layer_that_is_fixed = 0;
	int m_layers = 2;
	float m_height0 = 0;
	float m_height_step = 0.2f;

};

CLUMSY_ENUM_TAGGED_MORPHYSM_IMP_REGISTER(scene_maker_interface, scene_maker_type::stack_planes);
