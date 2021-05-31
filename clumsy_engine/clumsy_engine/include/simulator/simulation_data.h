

#pragma once

#include "signal.h" 
#include "clumsy_lib/type_list.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/type_map.h"
#include "clumsy_lib/variable_set.h"
#include "data_computes.h"
#include "matrix_math/matrix_math.h"

#include <vector>

namespace clumsy_engine
{

#define DEF_MEM(x,t,c,tl) DEF_DATA_CLASS_MEM(x,clumsy_lib::Dependent_Variable,t,c,tl)

	struct data
	{
		using vf = std::vector<float>;
		using vi = std::vector<int>;

		using v_v3f = std::vector<vec3f>;
		using v_v3i = std::vector<vec3i>;
		using v_vi = std::vector<std::vector<int>>;

		DEF_MEM(Gravity,					vec3f,		clumsy_lib::Plain_Computer,										clumsy_lib::empty_type_list);
		DEF_MEM(Time_Step,					float,		clumsy_lib::Plain_Computer,										clumsy_lib::empty_type_list);
		DEF_MEM(Mass_Density,				float,		clumsy_lib::Plain_Computer,										clumsy_lib::empty_type_list);
		DEF_MEM(Stretch_Stiff,				float,		clumsy_lib::Plain_Computer,										clumsy_lib::empty_type_list);
		DEF_MEM(Ref_Position,				v_v3f,		clumsy_lib::Plain_Computer,										clumsy_lib::empty_type_list);
		DEF_MEM(Position,					v_v3f,		clumsy_lib::Plain_Computer,										clumsy_lib::empty_type_list);
		DEF_MEM(Triangle_Indice,			vi,			clumsy_lib::Plain_Computer,										clumsy_lib::empty_type_list);

		DEF_MEM(Vertex_Num,					int,		TMP(Get_List_Size<Position, 1>),					clumsy_lib::type_list<Position>);
		DEF_MEM(Velocity,					v_v3f,		TMP(Allocate_With_Size<v_v3f, Vertex_Num, 1>),		clumsy_lib::type_list<Vertex_Num>);
		DEF_MEM(Last_Frame_Position,		v_v3f,		TMP(Allocate_With_Size<v_v3f, Vertex_Num, 1>),		clumsy_lib::type_list<Vertex_Num>);

		DEF_MEM(Triangle_Area,				vf,			Compute_Triangle_Area,								TMP(clumsy_lib::type_list<Ref_Position,Triangle_Indice>));
		DEF_MEM(Triangle_Normal,			v_v3f,		Compute_Triangle_Normal,							TMP(clumsy_lib::type_list<Position,Triangle_Indice>));

		DEF_MEM(Vertex_Area,				vf,			Compute_Vertex_Area,								TMP(clumsy_lib::type_list<Vertex_Num,Triangle_Area,Triangle_Indice>));
		DEF_MEM(Mass,						vf,			Compute_Mass,										TMP(clumsy_lib::type_list<Mass_Density,Vertex_Area>));
		DEF_MEM(Vertex_Adjacent_Triangle,   v_vi,		Compute_Vertex_Adjacent_Triangle,					TMP(clumsy_lib::type_list<Triangle_Indice,Vertex_Num>));
		DEF_MEM(Vertex_Normal,				v_v3f,		Compute_Vertex_Normal,								TMP(clumsy_lib::type_list<Triangle_Normal,Vertex_Adjacent_Triangle,Vertex_Num>));

		DEF_MEM(Edge_Indice,				vi,			Compute_Edge_Indices,								TMP(clumsy_lib::type_list<Triangle_Indice, Vertex_Num>));
		DEF_MEM(Edge_Length,				vf,			Compute_Edge_Length,								TMP(clumsy_lib::type_list<Ref_Position, Edge_Indice>));
		DEF_MEM(Delta_Position,				v_v3f,		Delta,												TMP(clumsy_lib::type_list< Position,Last_Frame_Position> ));
	} ;


}