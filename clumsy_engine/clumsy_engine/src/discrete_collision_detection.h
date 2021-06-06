
#pragma once  
#include "matrix_math/matrix_math.h"
#include "simulation_data.h"
#include "clumsy_lib/type_map.h"
#include "AABB.h"

namespace clumsy_engine
{
	class Discrete_Collision_Detection:
		public clumsy_lib::Variable_Acc_Constraint<clumsy_lib::type_list<data::Position, data::Edge_Indice, data::Stretch_Stiff>>
	{
	public:
		void construct_ee_bvh();

		void detection();

	private:
		int m_root;
		std::vector<vec3f> m_edge_representative_positions;
		std::vector<int> m_escape_indices;
		std::vector<AABB<3>> m_AABBs;
		std::vector<vec2i> m_children;

	};

}
