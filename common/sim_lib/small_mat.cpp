
module;


module sim_lib:small_mat;

import matrix_math;

namespace sim_lib
{
	using vec2f = matrix_math::matrix<float, 2, 1>;
	using vec3f = matrix_math::matrix<float, 3, 1>;
	using vec4f = matrix_math::matrix<float, 4, 1>;

	using mat2x2f = matrix_math::matrix<float, 2, 2 >;
	using mat3x3f = matrix_math::matrix<float, 3, 3 >;
	using mat4x4f = matrix_math::matrix<float, 4, 4 >;
	using mat2x4f = matrix_math::matrix<float, 2, 4 >;

	using mat3x2f = matrix_math::matrix<float, 3, 2 >;
	using mat2x3f = matrix_math::matrix<float, 2, 3 >;

}
