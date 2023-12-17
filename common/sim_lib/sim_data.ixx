
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

	export using float3 = std::array<float, 3>;
	export using int3 = std::array<int, 3>;
	export using int2 = std::array<int, 2>;

	export enum class solver_type
	{
		Dummy,
		Newton,
		Explicit,
	};

	struct no_validator
	{
		static bool apply(...) { return true; }
	};

	struct size_with
	{
		template<typename T>
		static bool apply(const std::vector<T>& in, int size) { return in.size() == size; }
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
	struct validator_fn
	{
		using fn = Fn;
		using params = clumsy_lib::type_list<P...>;
	};

	export struct sim_data
	{
		CE_ENTRY(solver,						CE_USE(type,	solver_type);				CE_VAL(init_val,	solver_type::Dummy);			CE_NIL(validator);																			CE_LIST(tags));
		CE_ENTRY(vertex_num,					CE_USE(type,	int);						CE_VAL(init_val,	-1);							CE_NIL(validator);																			CE_LIST(tags));
		CE_ENTRY(positions,						CE_USE(type,	std::vector<float3>);		CE_NIL(init_val);									CE_USE(validator,	validator_fn<size_with, vertex_num>);									CE_LIST(tags));
		CE_ENTRY(triangles,						CE_USE(type,	std::vector<int3>);			CE_NIL(init_val);									CE_USE(validator,	validator_fn<within_range, vertex_num>);								CE_LIST(tags));
		CE_ENTRY(obstacle_vert_index,			CE_USE(type,	std::vector<int>);			CE_NIL(init_val);									CE_USE(validator,	validator_fn<within_range, vertex_num>);								CE_LIST(tags));
		CE_ENTRY(stretch_edges,					CE_USE(type,	std::vector<int2>);			CE_NIL(init_val);									CE_USE(validator,	validator_fn<within_range, vertex_num>);								CE_LIST(tags));
		CE_ENTRY(gravity,						CE_USE(type,	float3);					CE_VAL(init_val,	float3{0,-10.f,0});				CE_NIL(validator);																			CE_LIST(tags));
		CE_ENTRY(time_step,						CE_USE(type,	float);						CE_VAL(init_val,	0.001f);						CE_NIL(validator);																			CE_LIST(tags));
		CE_ENTRY(density,						CE_USE(type,	float);						CE_VAL(init_val,	1e-2f);							CE_NIL(validator);																			CE_LIST(tags));

	};
}