module;

#include "matrix_math/matrix_math.h"

#include <vector>

module sim_lib : constraint_kernels;


namespace sim_lib
{
	struct inertial
	{

		template<typename write_lhs, typename write_rhs, typename vec_type>
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, float mass, float dt, const vec_type& gravity_accelaration, const vec_type& velocity)
		{
			float inertial_h = mass / dt / dt;
			lhs(0,0) = inertial_h;
			rhs(0) = mass * gravity_accelaration + inertial_h * (velocity * dt);
		}


	};

	struct edge_stretch
	{
		template<typename write_lhs, typename write_rhs, typename vec_type>
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, const std::array<vec_type, 2>& X, float stiff_modulus,float edge_length)
		{
			float w[]{ 1,-1 };
			auto dx = w[0] * X[0] + w[1] * X[1];
			float l = matrix_math::norm<2>::apply(dx);
			auto n = dx / l;

			float stiff = stiff_modulus / edge_length;

			auto stretch_f = -stiff * (l - edge_length) * n;

			rhs(0) += w[0] * stretch_f;
			rhs(1) += w[1] * stretch_f;

			lhs(0, 0) += w[0] * w[0] * stiff;
			lhs(1, 1) += w[1] * w[1] * stiff;

			lhs(0, 1) += w[0] * w[1] * stiff;
			lhs(1, 0) += w[1] * w[0] * stiff;
		}

	};


	struct triangle_stretch
	{

		template<typename write_lhs, typename write_rhs, typename vec_type,typename vec_type_u>
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, const std::array<vec_type, 3>& X, const std::array<vec_type_u, 3>& U, float stretch_stiff)
		{
			auto u0 = U[0] - U[2];
			auto u1 = U[1] - U[2];


			//dX=F*dU
			//XS=FUS
			//F=X S(US)^-1

		}

		template<typename write_lhs, typename write_rhs >
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, const std::array<vec3f, 3>& X, const std::array<vec2f, 3>& U, float stretch_stiff)
		{
			auto u0 = U[0];
			//auto u0 = U[0] - U[2];
			//auto u1 = U[1] - U[2];


			//dX=F*dU
			//XS=FUS
			//F=X S(US)^-1

		}
	};
}


