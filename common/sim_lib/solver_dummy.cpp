
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"

#include "clumsy_lib/field_defines.h"

#include "matrix_math/matrix_math.h"

#include <cmath>
#include <ranges>

module sim_lib : solver_dummy;

import :solver;
import :simulator_datas;
import matrix_math;

namespace sim_lib
{
	class dummy_solver :public solver_base
	{
	public:
		void update_data(const simulator_datas_t& sim_datas, const clumsy_lib::change_status_t& change_status) override
		{
			clumsy_lib::dependent_updater<var_list>::apply(m_datas, sim_datas, change_status);
		}

		void solve() override
		{

			auto& pos = m_datas.get_ref<var::positions>();

			m_linear_system.reserve_space( pos.size(),
				[](int vi)
				{
					return spm_t::r_c{vi, vi};
				}
			);

			auto writer_loop = m_linear_system.get_write_loop(pos.size(), [&](int i) { return std::vector<int>{ i }; });

			// TODO:
			float mass = m_datas.get_ref<var::density>();
			float dt = m_datas.get_ref < var::time_step>();
			vec3 g = m_datas.get_ref<var::gravity>();
			printf(" %f %f %f \n", g(0), g(1), g(2));
			std::vector<vec3> forces(pos.size());
			std::vector<vec3> dx(pos.size());
			std::vector<vec3> Ax(pos.size());
			std::vector<float> inv_diag(pos.size());

			auto get_inertial = [&](auto get_nz, int si)
				{
					forces[si] = mass * g;
					inv_diag[si] = dt * dt / mass;
					get_nz(si, 0, 0) = mass / dt / dt;
				};

			writer_loop(get_inertial);

			//jacobi
			for (int it = 0; it < 50; it++)
			{

				m_linear_system.for_each_nz(
					[&](int row, int col, const float& v)
					{
						Ax[row] += v * dx[col];
					}
				);

				for (int i = 0; i < dx.size(); i++)
				{
					dx[i] += inv_diag[i] * (forces[i] - Ax[i]);
				}
			}

			for (int i = 0; i < dx.size(); i++)
			{
				pos[i] = pos[i] + dx[i];
			}

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
			CE_SOLVER_DATA(gravity, vec3, simulator_data_update::assign, tl<simulator_datas::gravity>, tl<>)
			CE_SOLVER_DATA(time_step, float, simulator_data_update::assign, tl<simulator_datas::time_step>, tl<>)
			CE_SOLVER_DATA(density, float, simulator_data_update::assign, tl<simulator_datas::density>, tl<>)
		};

		using var_list = clumsy_lib::extract_member_type_list_t<var>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		using spm_t=matrix_math::sparse_matrix<float>;
		spm_t m_linear_system;
	};

}
