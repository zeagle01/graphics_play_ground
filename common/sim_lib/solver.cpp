
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"

#include "clumsy_lib/field_defines.h"


#include <cmath>

module sim_lib:solver;

import :simulator_datas;

import :solver_data_update;
import :small_mat;


namespace sim_lib
{

	using var_list = clumsy_lib::extract_member_type_list_t<simulator_datas>;
	using simulator_datas_t = clumsy_lib::Static_Type_Map<var_list>;

	class solver_base
	{

	public:

		virtual void update_data(const simulator_datas_t& sim_datas, const clumsy_lib::change_status_t& change_status) = 0;

		virtual void solve() = 0;

		virtual const std::vector<vec3>& get_result() const = 0;


		virtual const clumsy_lib::static_dep_graph& get_dep_graph() const
		{
			return {};
		}

	};

}

