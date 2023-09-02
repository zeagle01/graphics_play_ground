#pragma once

#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>
#include <array>

import quick_shell;
import sim_lib;


class App
{
public:
	void run();

private:
	void prepare_mesh();
	void init_sim();
	void init_sim_data();
	void update_sim_data();

private:

	void make_plane(float lx, float ly, int nx, int ny);

private:
	std::vector< int> indices;
	std::vector<float> pos;

	std::vector<int> fix_points = { 0 };
	bool m_sim_data_is_valid = false;
	std::string m_sim_data_is_str = "no";
private:
	std::array<float, 2> m_plane_size{ 0.5f,0.5f };
	std::array<int, 2> m_plane_resolution{ 2,2 };

private:
	std::string m_fps = "0";
	std::string m_sim_fps = "0";
	std::string m_render_fps = "0";
	quick_shell::main_window m;
	sim_lib::simulator sim;
};