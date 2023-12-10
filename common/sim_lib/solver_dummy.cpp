
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
			auto& vel = m_datas.get_ref<var::velocity>();
			auto& last_pos = m_datas.get_ref<var::last_positions>();

			//backup last pos
			last_pos = pos;

			for (int it = 0; it < 1; it++)
			{
				step();
			}

			float dt = 1e-3f;

			for (int i = 0; i < vel.size(); i++)
			{
				vel[i] = (pos[i] - last_pos[i]) / dt;
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
		void step()
		{

			auto& pos = m_datas.get_ref<var::positions>();
			auto& vel = m_datas.get_ref<var::velocity>();
			auto& last_pos = m_datas.get_ref<var::last_positions>();

			m_linear_system.reserve_space(pos.size(),
				[](int vi)
				{
					return spm_t::r_c{ vi, vi };
				}
			);

			m_linear_system.set_var_num(pos.size());

			m_linear_system.set_fixed_variables(m_datas.get_ref<var::fixed_verts>());

			//inertial
			const auto& dynamic_verts = m_datas.get_ref<var::dynamic_verts>();
			auto diag_loop = m_linear_system.get_write_loop(dynamic_verts.size(), [&](int i) { return std::vector<int>{ dynamic_verts[i] }; });

			float mass = m_datas.get_ref<var::density>();
			float dt = m_datas.get_ref < var::time_step>();
			vec3 g = m_datas.get_ref<var::gravity>();

			//float mass = 1e-2f;
			//float dt = 1e-3f;
			//vec3 g = { 0,-10.f,0 };

			std::vector<vec3> forces(pos.size());
			std::vector<vec3> dx(pos.size());
			std::vector<vec3> Ax(pos.size());
			std::vector<float> diag(pos.size());

			auto get_inertial = [&](auto get_nz, int si)
				{
					int v = dynamic_verts[si];
					forces[v] = mass * g;
					float inertial_h = mass / dt / dt;
					forces[v] += inertial_h * (vel[si] * dt);
					diag[v] = inertial_h;
					get_nz(si, 0, 0) = diag[si];
				};

			diag_loop(get_inertial);

			//edge stretch
			const auto& edges = m_datas.get_ref<var::stretch_edges>();
			const auto& edge_lengths = m_datas.get_ref<var::edge_lengths>();
			float stretch_stiff = 1e-1f;
			auto edge_loop = m_linear_system.get_write_loop(edges.size(), [&](int ei) { return std::vector<int>{ edges[ei][0], edges[ei][1] }; });

			auto add_stretch_edge_constraint = [&](auto get_nz, int ei)
				{
					int v0 = edges[ei][0];
					int v1 = edges[ei][1];
					std::array<vec3, 2> X{ pos[v0],pos[v1] };

					float w[]{ 1,-1 };
					auto dx = w[0] * X[0] + w[1] * X[1];
					float l = matrix_math::norm<2>::apply(dx);
					auto n = dx / l ;

					auto stretch_f = -stretch_stiff * (l - edge_lengths[ei]) * n;

					forces[v0] += w[0] * stretch_f;
					forces[v1] += w[1] * stretch_f;

					diag[v0] += w[0] * w[0] * stretch_stiff;
					get_nz(ei, 0, 0) += w[0] * w[0] * stretch_stiff;
					diag[v1] += w[1] * w[1] * stretch_stiff;
					get_nz(ei, 1, 1) += w[1] * w[1] * stretch_stiff;

					get_nz(ei, 0, 1) += w[0] * w[1] * stretch_stiff;
					get_nz(ei, 1, 0) += w[1] * w[0] * stretch_stiff;
				};

			edge_loop(add_stretch_edge_constraint);

			//jacobi
			for (int it = 0; it < 10; it++)
			{

				m_linear_system.for_each_nz(
					[&](int row, int col, const float& v)
					{
						Ax[row] += v * dx[col];
					}
				);

				for (int i = 0; i < dynamic_verts.size(); i++)
				{
					int v = dynamic_verts[i];
					dx[v] += (forces[v] - Ax[v]) / diag[v];
				}
			}

			for (int i = 0; i < pos.size(); i++)
			{
				pos[i] = pos[i] + dx[i];
			}

		}

	private:


#define CE_SOLVER_DATA(name,type,update_fn,dep_list,tag_list) CE_ADD_NODE(name, CE_TYPE(type) CE_FIELD(dep_update_fn,update_fn) CE_FIELD(deps,dep_list) CE_FIELD(tags,tag_list))

		template<typename ...T>
		using tl = clumsy_lib::type_list<T...>;
		struct var
		{
			CE_SOLVER_DATA(positions,			std::vector<vec3>,	solver_data_update::assign, tl<simulator_datas::positions>, tl<>)
			CE_SOLVER_DATA(velocity,			std::vector<vec3>,	simulator_data_update::resize, tl<simulator_datas::vert_size>, tl<>)
			CE_SOLVER_DATA(last_positions,		std::vector<vec3>,	simulator_data_update::resize, tl<simulator_datas::vert_size>, tl<>)

			CE_SOLVER_DATA(dynamic_verts,		std::vector<int>,	simulator_data_update::assign, tl<simulator_datas::dynamic_verts>, tl<>)
			CE_SOLVER_DATA(fixed_verts,			std::vector<int>,	simulator_data_update::assign, tl<simulator_datas::fixed_verts>, tl<>)

			CE_SOLVER_DATA(stretch_edges,		std::vector<int2>,	simulator_data_update::assign, tl<simulator_datas::stretch_edges>, tl<>)
			CE_SOLVER_DATA(edge_lengths,		std::vector<float>, simulator_data_update::assign, tl<simulator_datas::edge_lengths>, tl<>)

			CE_SOLVER_DATA(gravity,				vec3,				simulator_data_update::assign, tl<simulator_datas::gravity>, tl<>)
			CE_SOLVER_DATA(time_step,			float,				simulator_data_update::assign, tl<simulator_datas::time_step>, tl<>)
			CE_SOLVER_DATA(density,				float,				simulator_data_update::assign, tl<simulator_datas::density>, tl<>)
		};

		using var_list = clumsy_lib::extract_member_type_list_t<var>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		using spm_t=matrix_math::sparse_matrix<float>;
		spm_t m_linear_system;
	};

}
