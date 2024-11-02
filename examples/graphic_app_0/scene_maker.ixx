
module;

#include "clumsy_lib/enum_tagged_morphysm.h"
#include <vector> 
#include <memory>

export module  app:scene_maker;

import :matrix_using;

import clumsy_lib;
import quick_shell;

using namespace quick_shell;

export enum class scene_maker_type
{
	make_a_plane,
};

class scene_maker_interface;

export using scene_maker = clumsy_lib::enum_tagged_morphysm<scene_maker_interface, scene_maker_type>;



class scene_maker_interface
{
public:
	virtual bool make(
		std::vector< int>&indices,
		std::vector<int>&m_edges,
		std::vector<float>&pos,
		std::vector<float>&pos_2d,
		quick_shell::main_window & m
	) = 0;

		CLUMSY_ENUM_TAGGED_MORPHYSM(scene_maker_interface, scene_maker_type);
};

