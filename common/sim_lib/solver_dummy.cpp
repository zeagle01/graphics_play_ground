
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"

#include "clumsy_lib/field_defines.h"

#include "matrix_math/matrix_math.h"

#include <cmath>

module sim_lib : solver_dummy;

import :solver;
import :simulator_datas;

namespace sim_lib
{
	class dummy :public solver_base
	{
	public:
		void update_data(const simulator_datas_t& sim_datas, const clumsy_lib::change_status_t& change_status) override
		{
			clumsy_lib::dependent_updater<var_list>::apply(m_datas, sim_datas, change_status);
		}

		void solve() override
		{
			float amp = 0.01f;
			float freq = 1.0f;
			float step = 0.01f;

			auto& pos = m_datas.get_ref<var::positions>();
			int vNum = pos.size();
			for (int i = 0; i < vNum; i++)
			{
				pos[i](0) += amp * std::sin(t * freq);
			}

			t += step;

		}

		const std::vector<vec3>& get_result() const override
		{
			return m_datas.get_ref<var::positions>();
		}


		clumsy_lib::adj_list_t compute_dep_graph() override
		{
			return clumsy_lib::Dependent_Graph::build<var_list>();
		}

	private:


#define CE_SOLVER_DATA(name,type,update_fn,dep_list,tag_list) CE_ADD_NODE(name, CE_TYPE(type) CE_FIELD(dep_update_fn,update_fn) CE_FIELD(deps,dep_list) CE_FIELD(tags,tag_list))

		template<typename ...T>
		using tl = clumsy_lib::type_list<T...>;
		struct var
		{
			CE_SOLVER_DATA(positions, std::vector<vec3>, solver_data_update::assign, tl<simulator_datas::positions>, tl<>)
		};

		using var_list = clumsy_lib::extract_member_type_list_t<var>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		float t = 0.f;
	};

}
