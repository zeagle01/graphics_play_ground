
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
		template<typename T,int N>
		static bool apply(const std::vector<std::array<T,N>>& in, int size) 
		{
			for (int i = 0; i < in.size(); i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (in[i][j] >= size)
					{
						return false;
					}
				}
			}
			return true;
		}

		template<typename sizable>
		static bool apply(const std::vector<int>& in, const sizable& ref_container) 
		{
			int ref_size = ref_container.size();
			for (int i = 0; i < in.size(); i++)
			{
				if (in[i] >= ref_size)
				{
					return false;
				}
			}
			return true;
		}

	};

	export struct sim_data
	{

#define CE_SIM_DATA(name,def_type,def_default_value,def_validator,def_tags)	\
		struct name																			\
		{																					\
			using type =def_type;															\
			using default_value_fn=def_default_value;										\
			using validator_fn=validators<def_validator>;									\
			using tags=clumsy_lib::type_list<def_tags>;										\
		}; name* name##_var

#define CE_SIM_TYPE(...) __VA_ARGS__ 
#define CE_SIM_DEFAULT(...) EVAL(clumsy_lib::literal_t<__VA_ARGS__>)
#define CE_SIM_DEFAULT_NONE() nullptr_t
#define CE_SIM_VALIDATORS(...) __VA_ARGS__
#define CE_SIM_TAGS(...) __VA_ARGS__


		template<typename ...valid_fn>
		struct validators
		{
			static bool apply();
		};


		template<typename Fn,typename... P>
		struct validator
		{
			using fn = Fn;
			using params = clumsy_lib::type_list<P...>;
		};

		CE_SIM_DATA(solver,						CE_SIM_TYPE(solver_type),				CE_SIM_DEFAULT(solver_type::Dummy),			CE_SIM_VALIDATORS(),																		CE_SIM_TAGS());
		CE_SIM_DATA(vertex_num,					CE_SIM_TYPE(int),						CE_SIM_DEFAULT(-1),							CE_SIM_VALIDATORS(),																		CE_SIM_TAGS());
		CE_SIM_DATA(positions,					CE_SIM_TYPE(std::vector<float3>),		CE_SIM_DEFAULT_NONE(),						CE_SIM_VALIDATORS(validator<size_with, vertex_num>),										CE_SIM_TAGS());
		CE_SIM_DATA(triangles,					CE_SIM_TYPE(std::vector<int3>),			CE_SIM_DEFAULT_NONE(),						CE_SIM_VALIDATORS(validator<within_range, vertex_num>),										CE_SIM_TAGS());
		CE_SIM_DATA(obstacle_vert_index,		CE_SIM_TYPE(std::vector<int>),			CE_SIM_DEFAULT_NONE(),						CE_SIM_VALIDATORS(validator<within_range, vertex_num>),										CE_SIM_TAGS());
		CE_SIM_DATA(stretch_edges,				CE_SIM_TYPE(std::vector<int2>),			CE_SIM_DEFAULT_NONE(),						CE_SIM_VALIDATORS(validator<within_range, vertex_num>),										CE_SIM_TAGS());
		CE_SIM_DATA(gravity,					CE_SIM_TYPE(float3),					CE_SIM_DEFAULT(float3{0,-10.f,0}),			CE_SIM_VALIDATORS(),																		CE_SIM_TAGS());
		CE_SIM_DATA(time_step,					CE_SIM_TYPE(float),						CE_SIM_DEFAULT(0.001f),						CE_SIM_VALIDATORS(),																		CE_SIM_TAGS());
		CE_SIM_DATA(density,					CE_SIM_TYPE(float),						CE_SIM_DEFAULT(1e-2f),						CE_SIM_VALIDATORS(),																		CE_SIM_TAGS());



#undef CE_SIM_DATA	
#undef CE_SIM_TYPE 
#undef CE_SIM_DEFAULT
#undef CE_SIM_DEFAULT_NONE
#undef CE_SIM_VALIDATORS
#undef CE_SIM_TAGS

	};
}