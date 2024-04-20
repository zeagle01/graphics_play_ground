
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"

#include "clumsy_lib/field_defines.h"

#include "matrix_math/matrix_math.h"

#include <cmath>
#include <ranges>
#include <array>

module sim_lib : solver_dummy;

import :solver;
import :simulator_datas;
import :constraint_kernels;

import matrix_math;
import parallel_lib;

namespace sim_lib
{
	class dummy_solver :public solver_base
	{
	public:
		dummy_solver() 
		{
			m_dep_graph.build<var_list>();
		}

		void update_data(const simulator_datas_t& sim_datas, const clumsy_lib::change_status_t& change_status) override
		{
			clumsy_lib::dependent_updater<var_list>::apply(m_datas, sim_datas, change_status);
		}

		void solve() override
		{

			for (int it = 0; it < 1; it++)
			{
				step();
			}

			float dt = 1e-3f;
			auto& vel = m_datas.get_ref<var::velocity>();
			const auto& pos = m_datas.get_ref<var::positions>();
			const auto& last_pos = m_datas.get_ref<var::last_positions>();

			parallel::for_each(vel.size(), 256,
				[&](int i)
				{
					vel[i] = (pos[i] - last_pos[i]) / dt;
				}
			);

		}

		const std::vector<vec3>& get_result() const override
		{
			return m_datas.get_ref<var::positions>();
		}


		const clumsy_lib::static_dep_graph& get_dep_graph() const override
		{
			return m_dep_graph;
		}

	private:
		void step()
		{

			auto& pos = m_datas.get_ref<var::positions>();
			const auto& vel = m_datas.get_ref<var::velocity>();
			auto& last_pos = m_datas.get_ref<var::last_positions>();

			m_linear_system.set_variables_num(pos.size());

			m_linear_system.set_fixed_variables(m_datas.get_ref<var::fixed_verts>());

			//inertial
			const auto& dynamic_verts = m_datas.get_ref<var::dynamic_verts>();
			auto diag_loop = m_linear_system.get_write_loop(dynamic_verts.size(), [&](int i) { return std::vector<int>{ dynamic_verts[i] }; });

			float mass = m_datas.get_ref<var::density>();
			float dt = m_datas.get_ref < var::time_step>();
			vec3 g = m_datas.get_ref<var::gravity>();


			auto get_inertial = [&](auto lhs, auto rhs, int si)
				{
					int v = dynamic_verts[si];
					inertial::compute_elemnt(lhs, rhs, mass, dt, g, vel[v]);
				};

			diag_loop(get_inertial);

			//edge stretch
			const auto& edges = m_datas.get_ref<var::stretch_edges>();
			const auto& edge_lengths = m_datas.get_ref<var::edge_lengths>();
			const auto& stretch_edges_stiff = m_datas.get_ref<var::stretch_edges_stiff>();
			auto edge_loop = m_linear_system.get_write_loop(edges.size(), [&](int ei) { return std::vector<int>{ edges[ei][0], edges[ei][1] }; });

			auto add_stretch_edge_constraint = [&](auto lhs, auto rhs, int ei)
				{
					int v0 = edges[ei][0];
					int v1 = edges[ei][1];

					std::array<vec3, 2> X{  pos[v0],pos[v1] };
					//std::array<vec3f, 2> X{ vec3f{} ,vec3f{} };

					edge_stretch::compute_elemnt(lhs, rhs, X, stretch_edges_stiff[ei], edge_lengths[ei]);

					//remove this won't compile
					auto dx = 1.f * X[0];
				};

			edge_loop(add_stretch_edge_constraint);

			//face stretch
			//const auto& faces = m_datas.get_ref<var::stretch_faces>();

			//update fix pos
			std::vector<vec3> dx(pos.size());
			const auto& fixed_verts = m_datas.get_ref<var::fixed_verts>();
			for (int i = 0; i < fixed_verts.size(); i++)
			{
				int v = fixed_verts[i];
				dx[v] = pos[v] - last_pos[v];
			}
				
			//backup last pos
			last_pos = pos;

			//jacobi
			matrix_math::Jacobi_solver<float, vec3> linear_solver;
			linear_solver.solve(m_linear_system, dx);

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
			CE_SOLVER_DATA(positions,			std::vector<vec3>,	solver_data_update::assign,		tl<simulator_datas::positions>, tl<>)
			CE_SOLVER_DATA(velocity,			std::vector<vec3>,	simulator_data_update::resize,	tl<simulator_datas::vert_size>, tl<>)
			CE_SOLVER_DATA(last_positions,		std::vector<vec3>,	solver_data_update::assign,		tl<simulator_datas::positions>, tl<>)

			CE_SOLVER_DATA(dynamic_verts,		std::vector<int>,	simulator_data_update::assign, tl<simulator_datas::dynamic_verts>, tl<>)
			CE_SOLVER_DATA(fixed_verts,			std::vector<int>,	simulator_data_update::assign, tl<simulator_datas::fixed_verts>, tl<>)

			CE_SOLVER_DATA(stretch_edges,		std::vector<int2>,	simulator_data_update::assign, tl<simulator_datas::stretch_edges>, tl<>)
			CE_SOLVER_DATA(edge_lengths,		std::vector<float>, simulator_data_update::assign, tl<simulator_datas::edge_lengths>, tl<>)
			CE_SOLVER_DATA(stretch_edges_stiff,	std::vector<float>, simulator_data_update::assign, tl<simulator_datas::stretch_edges_stiff>, tl<>)

			CE_SOLVER_DATA(gravity,				vec3,				simulator_data_update::assign, tl<simulator_datas::gravity>, tl<>)
			CE_SOLVER_DATA(time_step,			float,				simulator_data_update::assign, tl<simulator_datas::time_step>, tl<>)
			CE_SOLVER_DATA(density,				float,				simulator_data_update::assign, tl<simulator_datas::density>, tl<>)
		};

		using var_list = clumsy_lib::extract_member_type_list_t<var>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		using spm_t = matrix_math::linear_system<float, vec3>;
		spm_t m_linear_system;


		clumsy_lib::static_dep_graph m_dep_graph;
	};

}
