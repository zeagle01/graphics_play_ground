
module;


export module matrix_math: matrix_alias;

import :matrix;

export
namespace matrix_math
{
	using vec2f = matrix<float, 2, 1>;
	using vec3f = matrix<float, 3, 1>;
	using vec4f = matrix<float, 4, 1>;

	using vec2i = matrix<int, 2, 1>;
	using vec3i = matrix<int, 3, 1>;
	using vec4i = matrix<int, 4, 1>;

	using mat2f = matrix<float, 2, 2>;
	using mat3f = matrix<float, 3, 3>;
	using mat4f = matrix<float, 4, 4>;
}
