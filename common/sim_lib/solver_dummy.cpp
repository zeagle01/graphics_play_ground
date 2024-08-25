
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"

#include "clumsy_lib/field_defines.h"

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
			backup_last();

			//newton step
			for (int it = 0; it < 1; it++)
			{
				solve_dx();

				step_forward();
			}

			update_velocity();
		}

		const std::vector<vec3f>& get_result() const override
		{
			return m_datas.get_ref<var::positions>();
		}


		const clumsy_lib::static_dep_graph& get_dep_graph() const override
		{
			return m_dep_graph;
		}

	private:
		void solve_dx()
		{
			init_linear_system();

			assemble_inertial();

			assemble_stretch();

			solve_fix_pos();

			solve_linear_system();

		}
	private:
		void solve_linear_system()
		{
			auto& dx = m_datas.get_ref<var::dx>();

			matrix_math::Jacobi_solver<mat3x3f, vec3f> linear_solver;
			linear_solver.solve(m_linear_system, dx);

		}

		void step_forward()
		{
			auto& pos = m_datas.get_ref<var::positions>();
			const auto& dx = m_datas.get_ref<var::dx>();
			parallel::for_each(pos.size(), 256,
				[&](int i)
				{
					pos[i] += dx[i];
				});
		}

		void update_velocity()
		{
			float dt = m_datas.get_ref < var::time_step>();
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

		void init_linear_system()
		{
			auto& last_pos = m_datas.get_ref<var::last_positions>();
			m_linear_system.set_variables_num(last_pos.size());
			m_linear_system.set_fixed_variables(m_datas.get_ref<var::fixed_verts>());
		}

		void solve_fix_pos()
		{
			auto& dx = m_datas.get_ref<var::dx>();
			const auto& fixed_verts = m_datas.get_ref<var::fixed_verts>();
			const auto& pos = m_datas.get_ref<var::positions>();
			const auto& target_pos = m_datas.get_ref<var::fixed_verts_pos>();

			parallel::for_each(fixed_verts.size(), 256,
				[&](int i)
				{
					int v = fixed_verts[i];
					dx[v] = target_pos[i] - pos[v];
				});
		}

		void backup_last()
		{
			const auto& pos = m_datas.get_ref<var::positions>();
			auto& last_pos = m_datas.get_ref<var::last_positions>();
			last_pos = pos;
		}

		void assemble_inertial()
		{

			const auto& vel = m_datas.get_ref<var::velocity>();
			const auto& dynamic_verts = m_datas.get_ref<var::dynamic_verts>();
			float mass = m_datas.get_ref<var::density>();
			float dt = m_datas.get_ref < var::time_step>();
			vec3f g = m_datas.get_ref<var::gravity>();
			auto get_inertial = [&](auto lhs, auto rhs, int si)
				{
					int v = dynamic_verts[si];
					inertial::compute_elemnt(lhs, rhs, mass, dt, g, vel[v]);
				};

			auto diag_loop = m_linear_system.get_write_loop(dynamic_verts.size(), [&](int i) { return std::vector<int>{ dynamic_verts[i] }; });
			diag_loop(get_inertial);
		}

		void assemble_stretch()
		{
			//edge stretch
			{
				const auto& pos = m_datas.get_ref<var::positions>();
				const auto& edges = m_datas.get_ref<var::stretch_edges>();
				const auto& edge_lengths = m_datas.get_ref<var::edge_lengths>();
				const auto& stretch_edges_stiff = m_datas.get_ref<var::stretch_edges_stiff>();
				auto add_stretch_edge_constraint = [&](auto lhs, auto rhs, int ei)
					{
						int v0 = edges[ei][0];
						int v1 = edges[ei][1];

						mat3x2f X = matrix_math::from_columns(pos[v0], pos[v1]);

						edge_stretch::compute_elemnt(lhs, rhs, X, stretch_edges_stiff[ei], edge_lengths[ei]);
					};

				auto edge_loop = m_linear_system.get_write_loop(edges.size(), [&](int ei) { return std::vector<int>{ edges[ei][0], edges[ei][1] }; });
				edge_loop(add_stretch_edge_constraint);

			}


			//face stretch
			{
				const auto& pos = m_datas.get_ref<var::positions>();
				const auto& material_pos = m_datas.get_ref<var::material_positions>();
				const auto& faces = m_datas.get_ref<var::stretch_triangles>();
				const auto& triangle_stiff = m_datas.get_ref<var::stretch_triangles_stiff>();
				auto add_stretch_triangles_constraint = [&](auto lhs, auto rhs, int ti)
					{
						int v0 = faces[ti][0];
						int v1 = faces[ti][1];
						int v2 = faces[ti][2];

						mat3x3f X = matrix_math::from_columns(pos[v0], pos[v1], pos[v2]);
						mat2x3f U = matrix_math::from_columns(material_pos[v0], material_pos[v1], material_pos[v2]);

						triangle_stretch::compute_elemnt(lhs, rhs, X, U, triangle_stiff[ti]);
					};

				auto triangle_loop = m_linear_system.get_write_loop(faces.size(), [&](int ti) { return std::vector<int>{ faces[ti][0], faces[ti][1], faces[ti][2] }; });
				triangle_loop(add_stretch_triangles_constraint);

			}

		}

	private:


#define CE_SOLVER_DATA(name,type,update_fn,dep_list,tag_list) CE_ADD_NODE(name, CE_TYPE(type) CE_FIELD(dep_update_fn,update_fn) CE_FIELD(deps,dep_list) CE_FIELD(tags,tag_list))

		template<typename ...T>
		using tl = clumsy_lib::type_list<T...>;
		struct var
		{
			CE_SOLVER_DATA(positions,							std::vector<vec3f>,						solver_data_update::assign,		tl<simulator_datas::positions>, tl<>)
			CE_SOLVER_DATA(rest_positions,						std::vector<vec3f>,						solver_data_update::assign,		tl<simulator_datas::rest_positions>, tl<>)
			CE_SOLVER_DATA(material_positions,					std::vector<vec2f>,						solver_data_update::assign,		tl<simulator_datas::material_positions>, tl<>)
			CE_SOLVER_DATA(velocity,							std::vector<vec3f>,						simulator_data_update::resize,	tl<simulator_datas::vert_size>, tl<>)
			CE_SOLVER_DATA(last_positions,						std::vector<vec3f>,						solver_data_update::assign,		tl<simulator_datas::positions>, tl<>)
			CE_SOLVER_DATA(dx,									std::vector<vec3f>,						simulator_data_update::resize,	tl<simulator_datas::vert_size>, tl<>)

			CE_SOLVER_DATA(dynamic_verts,						std::vector<int>,						simulator_data_update::assign, tl<simulator_datas::dynamic_verts>, tl<>)
			CE_SOLVER_DATA(fixed_verts,							std::vector<int>,						simulator_data_update::assign, tl<simulator_datas::fixed_verts>, tl<>)
			CE_SOLVER_DATA(fixed_verts_pos,						std::vector<vec3f>,						simulator_data_update::assign, tl<simulator_datas::fixed_verts_pos>, tl<>)

			CE_SOLVER_DATA(stretch_edges,						std::vector<int2>,						simulator_data_update::assign, tl<simulator_datas::stretch_edges>, tl<>)
			CE_SOLVER_DATA(stretch_edges_stiff,					std::vector<float>,						simulator_data_update::assign, tl<simulator_datas::stretch_edges_stiff>, tl<>)
			CE_SOLVER_DATA(edge_lengths,						std::vector<float>,						simulator_data_update::assign, tl<simulator_datas::edge_lengths>, tl<>)


			CE_SOLVER_DATA(stretch_triangles,					std::vector<int3>,						simulator_data_update::assign, tl<simulator_datas::stretch_triangles>, tl<>);
			CE_SOLVER_DATA(stretch_triangles_stiff,				std::vector<triangle_stretch_stiff>,	simulator_data_update::assign, tl<simulator_datas::stretch_triangles_stiff>, tl<>);


			CE_SOLVER_DATA(gravity,								vec3f,									simulator_data_update::assign, tl<simulator_datas::gravity>, tl<>)
			CE_SOLVER_DATA(time_step,							float,									simulator_data_update::assign, tl<simulator_datas::time_step>, tl<>)
			CE_SOLVER_DATA(density,								float,									simulator_data_update::assign, tl<simulator_datas::density>, tl<>)
		};

		using var_list = clumsy_lib::extract_member_type_list_t<var>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		using spm_t = matrix_math::linear_system<mat3x3f, vec3f>;
		spm_t m_linear_system;


		clumsy_lib::static_dep_graph m_dep_graph;
	};

}
