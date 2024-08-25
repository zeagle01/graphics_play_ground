module;

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

module sim_lib : constraint_kernels;

import matrix_math;
import :small_mat;
import :sim_data;



namespace sim_lib
{
	struct inertial
	{

		template<typename write_lhs, typename write_rhs >
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, float mass, float dt, const vec3f& gravity_accelaration, const vec3f& velocity)
		{
			float inertial_h = mass / dt / dt;

			mat3x3f I = matrix_math::identity<float, 3>();

			lhs(0, 0) = inertial_h * I;
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

			mat3x3f I = matrix_math::identity<float, 3>();

			for (int i = 0; i < 2; i++)
			{
				rhs(i) += w(i) * stretch_f;

				for (int j = 0; j < 2; j++)
				{
					lhs(i, j) += w(i) * w(j) * stiff * I;
				}
			}
		}

	};


	struct triangle_stretch
	{

		template<typename write_lhs, typename write_rhs >
		static void compute_elemnt(write_lhs lhs, write_rhs rhs, const mat3x3f& X, const mat2x3f& U, const triangle_stretch_stiff& stiff)
		{
			constexpr mat3x2f S({
				1.f,0.f,
				0.f,1.f,
				-1.f,-1.f
				});

			mat2x2f I2 = matrix_math::identity<float, 2>();

			mat3x3f I3 = matrix_math::identity<float, 3>();

			float c = 0.5f * std::sqrt(2.f);
			float s = c;

			mat2x2f orientation[] = {

				I2,

				mat2x2f(
				{
					c,-s,
					s,c
				}),
			};
			auto dU = U * S;

			float area = 0.5f * std::abs(matrix_math::det(dU));

			float k[] = { stiff.young_x ,stiff.young_y };

			for (int orientation_i = 0; orientation_i < 2; orientation_i++)
			{
				const auto& R = orientation[orientation_i];

				auto W = S * (I2 / (R * U * S));

				for (int col_i = 0; col_i < 2; col_i++)
				{
					const auto& w = column(W, col_i);

					auto dx = X * w;

					float l = matrix_math::length(dx);

					float l0 = 1.f;

					float C = l - l0;

					float epsilon = C / l;

					float clamped_epsilon = epsilon < 0.f ? 0.f : epsilon;

					auto n = dx / l;

					auto nnT = n * transpose(n);

					for (int i = 0; i < 3; i++)
					{
						rhs(i) += -area * k[col_i] * C * w(i) * n;

						for (int j = 0; j < 3; j++)
						{
							lhs(i, j) += area * k[col_i] * w(i) * w(j) * (nnT + clamped_epsilon * (I3 - nnT));
						}
					}
				}

			}

		}

	};
}


