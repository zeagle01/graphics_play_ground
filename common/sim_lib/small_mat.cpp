
module;


module sim_lib:small_mat;

import matrix_math;

namespace sim_lib
{
	using vec3 = matrix_math::matrix<float, 3, 1>;
	using vec2 = matrix_math::matrix<float, 2, 1>;
	using mat3 = matrix_math::matrix<float, 3, 3 >;
	using mat3x2 = matrix_math::matrix<float, 3, 2 >;

}
