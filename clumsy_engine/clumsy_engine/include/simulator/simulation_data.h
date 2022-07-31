

#pragma once

#include "clumsy_lib/type_list.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/type_map.h"
#include "dependent_variable_set.h"
#include "data_computes.h"
#include "matrix_math/matrix_math.h"

#include <vector>

using namespace matrix_math;

namespace clumsy_engine
{

#define DEF_MEM(x,t,c,tl) ADD_TYPE_TO_GROUP(x,Dependent_Variable,t,c,tl)

#define DEPENDENT_TYPE_LIST(...) CE_WRAP(clumsy_lib::type_list<__VA_ARGS__>)

	struct data
	{
		using vf = std::vector<float>;
		using vi = std::vector<int>;

		using v_v3f = std::vector<vec3f>;
		using v_v3i = std::vector<vec3i>;
		using v_vi = std::vector<std::vector<int>>;

		DEF_MEM(Gravity_Acceleration,		vec3f,				Plain_Computer,										DEPENDENT_TYPE_LIST()																		);
		DEF_MEM(Time_Step,					float,				Plain_Computer,										DEPENDENT_TYPE_LIST()																		);
		DEF_MEM(Mass_Density,				float,				Plain_Computer,										DEPENDENT_TYPE_LIST()																		);
		DEF_MEM(Stretch_Stiff,				float,				Plain_Computer,										DEPENDENT_TYPE_LIST()																		);
		DEF_MEM(Ref_Position,				v_v3f,				Plain_Computer,										DEPENDENT_TYPE_LIST()																		);
		DEF_MEM(Position,					v_v3f,				Plain_Computer,										DEPENDENT_TYPE_LIST()																		);
		DEF_MEM(Triangle_Indice,			vi,					Plain_Computer,										DEPENDENT_TYPE_LIST()																		);

		DEF_MEM(Vertex_Num,					int,				CE_WRAP(Get_List_Size<Position, 1>),							DEPENDENT_TYPE_LIST(Position)																);
		DEF_MEM(Velocity,					v_v3f,				CE_WRAP(Allocate_With_Size<v_v3f, Vertex_Num, 1>),				DEPENDENT_TYPE_LIST(Vertex_Num)																);
		DEF_MEM(Last_Frame_Position,		v_v3f,				CE_WRAP(Allocate_With_Size<v_v3f, Vertex_Num, 1>),				DEPENDENT_TYPE_LIST(Vertex_Num)																);

		DEF_MEM(Triangle_Area,				vf,					Compute_Triangle_Area,											DEPENDENT_TYPE_LIST(Ref_Position,Triangle_Indice)											);
		DEF_MEM(Triangle_Normal,			v_v3f,				Compute_Triangle_Normal,										DEPENDENT_TYPE_LIST(Position,Triangle_Indice)												);

		DEF_MEM(Vertex_Area,				vf,					Compute_Vertex_Area,											DEPENDENT_TYPE_LIST(Vertex_Num,Triangle_Area,Triangle_Indice)								);
		DEF_MEM(Mass,						vf,					Compute_Mass,													DEPENDENT_TYPE_LIST(Mass_Density,Vertex_Area)												);
		DEF_MEM(Vertex_Adjacent_Triangle,   v_vi,				Compute_Vertex_Adjacent_Triangle,								DEPENDENT_TYPE_LIST(Triangle_Indice,Vertex_Num)												);
		DEF_MEM(Vertex_Normal,				v_v3f,				Compute_Vertex_Normal,											DEPENDENT_TYPE_LIST(Triangle_Normal,Vertex_Adjacent_Triangle,Vertex_Num)					);

		DEF_MEM(Edge_Indice,				vi,					Compute_Edge_Indices,											DEPENDENT_TYPE_LIST(Triangle_Indice, Vertex_Num)											);
		DEF_MEM(Edge_Adjacent_Triangle,		vi,					Compute_Edge_Adjacent_Triangle,									DEPENDENT_TYPE_LIST(Triangle_Indice, Vertex_Num)											);
		DEF_MEM(Edge_Length,				vf,					Compute_Edge_Length,											DEPENDENT_TYPE_LIST(Ref_Position, Edge_Indice)												);
		DEF_MEM(Delta_Position,				v_v3f,				Delta,															DEPENDENT_TYPE_LIST( Position,Last_Frame_Position )											);
	} ;


}