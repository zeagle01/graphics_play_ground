
module;

#include "clumsy_lib/field_defines.h" 
#include "clumsy_lib/macro_loop.h"
#include "clumsy_lib/type_list.h"

#include <vector> 
#include <array>

export module sim_lib: sim_data;


namespace sim_lib
{

	export using float3 = std::array<float, 3>;
	export using int3 = std::array<int, 3>;

	export struct stretch_data
	{
		int triangle_index;
		float stiff;
	};

	export struct bending_data
	{
		int v0,v1;
		float stiff;
		float rest_angle;
	};

	export struct self_collision_data
	{
		int triangle_index;
		float stiff;
		float thickness;
	};

	export struct obstacle_data
	{
		float stiff;
		float thickness;
		float3 velocity;
	};

	export enum class solver_type
	{
		Dummy,
		Newton,
		Explicit,
	};

	enum class optional
	{
		no,
		yes
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

	};

	template<typename name_t, typename T, typename validator_t = no_validator, optional defaulv_valid = optional::no>
	struct valid_wrapper
	{
		using type = name_t;
		T data;
		bool is_valid = bool(defaulv_valid);
		using validator = validator_t;
	};


#define SIM_TYPE(t) CE_TYPE(EVAL(t))
#define CE_SIM_INTERFACE_DATA(name,t,validator,deplist,optional) CE_ADD_NODE(name, SIM_TYPE(valid_wrapper<EVAL(name,t, validator, optional)>) CE_FIELD(deps, deplist))

	template<typename ...T>
	using tl = clumsy_lib::type_list<T...>;

	export struct sim_data
	{
		CE_SIM_INTERFACE_DATA(solver,		solver_type,			no_validator, tl<>,			optional::no);

		CE_SIM_INTERFACE_DATA(vertex_num,	int,					no_validator, tl<>,			optional::no);

		CE_SIM_INTERFACE_DATA(positions,	std::vector<float3>,	size_with, tl<vertex_num>,	optional::no);

		CE_SIM_INTERFACE_DATA(triangles,	std::vector<int3>,		within_range, tl<vertex_num>,	optional::yes);

		CE_SIM_INTERFACE_DATA(obstacle_vert_index,	std::vector<int>,no_validator, tl<>,	optional::yes);

		//CE_ADD_NODE(stretch, CE_TYPE(std::vector<stretch_data>));

		//CE_ADD_NODE(bending, CE_TYPE(std::vector<bending_data>));

		//CE_ADD_NODE(self_collision, CE_TYPE(std::vector<self_collision_data>));


		CE_SIM_INTERFACE_DATA(gravity,	float3,	no_validator, tl<>,	optional::yes);



	};

}