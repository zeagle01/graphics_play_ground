module;

#include <vector>

module sim_lib : constraint_kernels;

import matrix_math;
import :small_mat;



namespace sim_lib
{
	struct inertial
	{

		template<typename write_lhs, typename write_rhs >
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, float mass, float dt, const vec3f& gravity_accelaration, const vec3f& velocity)
		{
			float inertial_h = mass / dt / dt;
			lhs(0,0) = inertial_h;
			rhs(0) = mass * gravity_accelaration + inertial_h * (dt * velocity);
		}


	};

	struct edge_stretch
	{
		template<typename write_lhs, typename write_rhs >
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, const mat3x2f& X, float stiff_modulus, float edge_length)
		{
			vec2f w({ 1,-1 });
			auto dx = X * w;
			float l = matrix_math::length(dx);
			auto n = dx / l;

			float stiff = stiff_modulus / edge_length;

			auto stretch_f = -stiff * (l - edge_length) * n;

			rhs(0) += w(0) * stretch_f;
			rhs(1) += w(1) * stretch_f;

			lhs(0, 0) += w(0) * w(0) * stiff;
			lhs(1, 1) += w(1) * w(1) * stiff;

			lhs(0, 1) += w(0) * w(1) * stiff;
			lhs(1, 0) += w(1) * w(0) * stiff;
		}

	};


	struct triangle_stretch
	{

		template<typename write_lhs, typename write_rhs >
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, const mat3x3f& X, const mat2x3f& U, float stretch_stiff)
		{
			constexpr mat3x2f S({
				1.f,0.f,
				0.f,1.f,
				-1.f,-1.f
				});

			mat2x2f I{};

			auto F = X * S * (I / (U * S));

		}

	};
}


