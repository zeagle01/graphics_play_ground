#pragma once

#include <iostream>
#include <ranges>
#include <algorithm>
#include <vector>

import quick_shell;
import sim_lib;


class App
{
public:
	void run();

private:
	void prepare_mesh();
	void init_sim();

private:

	void make_plane(float lx, float ly, int nx, int ny);

	std::vector< int> indices;
	std::vector<float> pos;

	quick_shell::main_window m;

	sim_lib::simulator sim;
};