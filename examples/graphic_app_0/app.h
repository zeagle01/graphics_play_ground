#pragma once

#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <array>
#include "mouse_picker.h"

import quick_shell;
import sim_lib;
import app;


class App
{
public:
	void run();

private:
	void register_update_fn();
	void register_event_fn();

private:
	void init();
	void init_sim_data();

private:
	struct FPS_wrapper;

private:
	void update();
private:
	bool make_scene();
	void animate(bool geometry_changed);
	void render();

private:
	void connect_sim_ui();
	void connect_render_ui();
	void show_fps();

private:
	void pick(int x, int y);
	void drag(int x, int y);
	void release_pick();

	//plane maker
private:
	scene_maker m_scene_maker;
	scene_maker_type m_scene_maker_type{};

	//mouse picker
private:
	mouse_picker m_mouse_picker;

	picked_info m_picked { -1,{} };
	struct mouse_ray { std::array<float, 3> p, dir; };
	mouse_ray m_mouse_ray;

	//simulator
private:
	std::vector< int> indices;
	std::vector< int> m_edges;
	std::vector<float> pos;
	std::vector<float> pos_2d;

	bool m_sim_data_is_valid = false;
	std::string m_sim_data_is_str = "no";

	float uniform_edge_stretch_stiff = 1e2f;
	sim_lib::triangle_stretch_stiff uniform_triangle_stretch_stiff = { 1e2f,1e2f,1e2f,1e2f };

private:
	std::array<float, 2> m_plane_size{ 0.5f,0.5f };
	std::array<int, 2> m_plane_resolution{ 10,10 };

	using float3 = std::array<float, 3>;
	std::vector<int> m_preset_fix_points;
	std::vector<float3> m_preset_fix_points_pos;
	std::vector<int> m_current_fix_points;
	std::vector<float3> m_current_fix_points_pos;

private:
	bool m_enable_edge_stretch_constraint = false;
	bool m_enable_face_stretch_constraint = true;

private:
	bool m_enable_fill = true;
	bool m_enable_wirefram = true;

private:
	std::string m_fps = "0";
	std::string m_sim_fps = "0";
	std::string m_render_fps = "0";
	quick_shell::main_window m;
	sim_lib::simulator sim;



};