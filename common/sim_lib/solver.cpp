
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"

#include "clumsy_lib/field_defines.h"

#include "matrix_math/matrix_math.h"

#include <cmath>

module sim_lib:solver;

import :sim_data;

import :solver_data_update;


namespace sim_lib
{
	//
	using vec3 = matrix_math::mat<3, 1, float>;
	using mat3 = matrix_math::mat<3, 3, float>;



	using var_list = clumsy_lib::extract_member_type_list_t<sim_data>;
	using simulator_datas_t = clumsy_lib::Static_Type_Map<var_list>;

	class solver_base
	{

	public:

		virtual void update_data(const simulator_datas_t& sim_datas, const clumsy_lib::change_status_t& change_status) = 0;

		virtual void solve() = 0;

		virtual const std::vector<vec3>& get_result() const = 0;

		virtual clumsy_lib::adj_list_t compute_dep_graph()
		{
			return {};
		}

	};

}

