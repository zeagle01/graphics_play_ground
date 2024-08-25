
module;

#include "clumsy_lib/field_defines.h" 
#include "clumsy_lib/macro_loop.h"
#include "clumsy_lib/type_list.h"

#include <vector> 
#include <array>

export module sim_lib: sim_data;

import clumsy_lib;

namespace sim_lib
{

	export using float2 = std::array<float, 2>;
	export using float3 = std::array<float, 3>;
	export using int3 = std::array<int, 3>;
	export using int2 = std::array<int, 2>;

	export struct triangle_stretch_stiff
	{
		float young_x;
		float young_y;
		float possion_ratio;
		float shear_modulus;
	};

	export enum class solver_type
	{
		Dummy,
		Newton,
		Explicit,
	};


	struct size_with
	{
		template<typename T>
		static bool apply(const std::vector<T>& in, int size) { return in.size() == size; }

		template<typename T, typename U>
		static bool apply(const std::vector<T>& out, const std::vector<U>& in) { return in.size() == out.size(); }
	};

	template<auto v>
	struct greater_than
	{
		template<typename T>
		static bool apply(const T& in) { return in > v; }
	};

	struct no_repeat
	{
		template<typename T,int N>
		static bool apply(const std::vector<std::array<T, N>>& in)
		{
			for (int i = 0; i < in.size(); i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (in[i][j] == in[i][(j + 1) % N])
					{
						return false;
					}
				}
			}

			return true;
		}
	};

	struct within_range
	{

		template<typename T>
		static bool apply(const std::vector<T>& in, const int& size)
		{
			for (int i = 0; i < in.size(); i++)
			{
				if (in[i] >= size || in[i] < 0)
				{
					return false;
				}
			}
			return true;

		}

		template<typename T, typename U>
		static bool apply(const std::vector<T>& in, const std::vector<U>& ref_vec)
		{
			int size = ref_vec.size();
			for (int i = 0; i < in.size(); i++)
			{
				if (in[i] >= size || in[i] < 0)
				{
					return false;
				}
			}
			return true;

		}

		template<typename T,int N>
		static bool apply(const std::vector<std::array<T,N>>& in, const int& size) 
		{
			for (int i = 0; i < in.size(); i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (in[i][j] >= size || in[i][j] < 0)
					{
						return false;
					}
				}
			}
			return true;
		}
	};

	template<typename Fn, typename... P>
	struct fn
	{
		using name = Fn;
		using params = clumsy_lib::type_list<P...>;
	};


	export struct sim_data
	{
		CE_ENTRY(solver,						CE_USE(type,	solver_type);						CE_VAL(init_val,	solver_type::Dummy);			CE_LIST(validator);																CE_LIST(tags));
		CE_ENTRY(vertex_num,					CE_USE(type,	int);								CE_VAL(init_val,	-1);							CE_LIST(validator);																CE_LIST(tags));
		CE_ENTRY(positions,						CE_USE(type,	std::vector<float3>);				CE_NIL(init_val);									CE_LIST(validator,	fn<size_with, vertex_num>);									CE_LIST(tags));
		CE_ENTRY(rest_positions,				CE_USE(type,	std::vector<float3>);				CE_NIL(init_val);									CE_LIST(validator,	fn<size_with, vertex_num>);									CE_LIST(tags));
		CE_ENTRY(material_positions,			CE_USE(type,	std::vector<float2>);				CE_NIL(init_val);									CE_LIST(validator,	fn<size_with, vertex_num>);									CE_LIST(tags));

		CE_ENTRY(triangles,						CE_USE(type,	std::vector<int3>);					CE_NIL(init_val);									CE_LIST(validator,	fn<within_range, vertex_num>,fn<no_repeat>);				CE_LIST(tags));

		CE_ENTRY(obstacle_vert_index,			CE_USE(type,	std::vector<int>);					CE_NIL(init_val);									CE_LIST(validator,	fn<within_range, vertex_num>);								CE_LIST(tags));
		CE_ENTRY(obstacle_vert_pos,				CE_USE(type,	std::vector<float3>);				CE_NIL(init_val);									CE_LIST(validator,	fn<size_with, obstacle_vert_index>);						CE_LIST(tags));

		CE_ENTRY(stretch_edges,					CE_USE(type,	std::vector<int2>);					CE_NIL(init_val);									CE_LIST(validator,	fn<within_range, vertex_num>);								CE_LIST(tags));
		CE_ENTRY(stretch_edges_stiff,			CE_USE(type,	std::vector<float>);				CE_NIL(init_val);									CE_LIST(validator,	fn<size_with, stretch_edges>);								CE_LIST(tags));

		CE_ENTRY(stretch_triangles,				CE_USE(type,	std::vector<int>);					CE_NIL(init_val);									CE_LIST(validator,	fn<within_range, triangles>);								CE_LIST(tags));
		CE_ENTRY(stretch_triangles_stiff,		CE_USE(type,	std::vector<triangle_stretch_stiff>);CE_NIL(init_val);									CE_LIST(validator,	fn<size_with, stretch_triangles>);							CE_LIST(tags));

		CE_ENTRY(gravity,						CE_USE(type,	float3);							CE_VAL(init_val,	float3{0,-10.f,0});				CE_LIST(validator);																CE_LIST(tags));
		CE_ENTRY(time_step,						CE_USE(type,	float);								CE_VAL(init_val,	0.01f);							CE_LIST(validator,  fn<greater_than<0.f>>);										CE_LIST(tags));
		CE_ENTRY(density,						CE_USE(type,	float);								CE_VAL(init_val,	1e-1f);							CE_LIST(validator,  fn<greater_than<0.f>>);										CE_LIST(tags));

	};
}