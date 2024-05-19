
#include "gmock/gmock.h"
#include <initializer_list>

using namespace testing;


import matrix_math;

using namespace matrix_math;


using mat2x2 = matrix_math::matrix<float, 2, 2>;
using mat2x3 = matrix_math::matrix<float, 2, 3>;
using mat3x2 = matrix_math::matrix<float, 3, 2>;
using vec2 = matrix_math::matrix<float, 2, 1>;
using tvec2 = matrix_math::matrix<float, 1, 2>;
using vec4 = matrix_math::matrix<float, 4, 1>;
using vec6 = matrix_math::matrix<float, 6, 1>;
using tvec6 = matrix_math::matrix<float, 1, 6>;


//mat init and acess
TEST(matrix_test, init_with_zeros)
{
	mat2x2 m{};

	EXPECT_THAT(m(0, 0), Eq(0.f));
	EXPECT_THAT(m(1, 0), Eq(0.f));
	EXPECT_THAT(m(0, 1), Eq(0.f));
	EXPECT_THAT(m(1, 1), Eq(0.f));

}

TEST(matrix_test, default_init_with_constexpr)
{
	constexpr mat2x2 m{};

	EXPECT_THAT(m(0, 0), Eq(0.f));
	EXPECT_THAT(m(1, 0), Eq(0.f));
	EXPECT_THAT(m(0, 1), Eq(0.f));
	EXPECT_THAT(m(1, 1), Eq(0.f));

}

TEST(matrix_test, init_with_values)
{
	mat2x2 m
	{
		1,3,
		2,4 
	};

	EXPECT_THAT(m(0, 0), Eq(1.f));
	EXPECT_THAT(m(1, 0), Eq(2.f));
	EXPECT_THAT(m(0, 1), Eq(3.f));
	EXPECT_THAT(m(1, 1), Eq(4.f));

}

TEST(matrix_test, init_with_values_constexpr)
{
	constexpr mat2x2 m
	{
		1,3,
		2,4 
	};

	EXPECT_THAT(m(0, 0), Eq(1.f));
	EXPECT_THAT(m(1, 0), Eq(2.f));
	EXPECT_THAT(m(0, 1), Eq(3.f));
	EXPECT_THAT(m(1, 1), Eq(4.f));

}

TEST(matrix_test, assign_component)
{
	mat2x2 m{};
	m(0, 0) = 42.f;

	EXPECT_THAT(m(0, 0), Eq(42.f));

}

//vector access
template<typename T>
concept can_access_with_two_index = requires (T v) { v(0, 0); };

template<typename T>
static constexpr  bool can_access_with_two_index_v = can_access_with_two_index<T>;

TEST(matrix_test, vec_cant_not_access_with_two_index)
{
	constexpr bool can_access_with_two_index = can_access_with_two_index_v<vec2>;
	EXPECT_FALSE(can_access_with_two_index);
}

TEST(matrix_test, vec_access_with_one_index)
{
	vec2 v{};

	EXPECT_THAT(v(0), Eq(0.f));
	EXPECT_THAT(v(1), Eq(0.f));
}

TEST(matrix_test, vec_assign_with_one_index)
{
	vec2 v{};
	v(0) = 42.f;
	EXPECT_THAT(v(0), Eq(42.f));
	EXPECT_THAT(v(1), Eq(0.f));
}

//view
TEST(matrix_test, transpose_access)
{
	mat2x3 m
	{
		1,3,5,
		2,4,6
	};

	auto mt = transpose(m);

	EXPECT_THAT(mt(0,0), Eq(1.f));
	EXPECT_THAT(mt(1,0), Eq(3.f));
	EXPECT_THAT(mt(2,0), Eq(5.f));
	EXPECT_THAT(mt(0,1), Eq(2.f));
	EXPECT_THAT(mt(1,1), Eq(4.f));
	EXPECT_THAT(mt(2,1), Eq(6.f));

}

TEST(matrix_test, transpose_assign)
{
	mat2x3 m
	{
		1,3,5,
		2,4,6
	};

	transpose(m)(0, 1) = 42.f;

	EXPECT_THAT(m(0,0), Eq(1.f));
	EXPECT_THAT(m(1,0), Eq(42.f));
	EXPECT_THAT(m(0,1), Eq(3.f));
	EXPECT_THAT(m(1,1), Eq(4.f));
	EXPECT_THAT(m(0,2), Eq(5.f));
	EXPECT_THAT(m(1,2), Eq(6.f));
}

TEST(matrix_test, transpose_assign_whole)
{
	mat2x3 m
	{
		1,3,5,
		2,4,6
	};

	mat3x2 m0
	{
		1,4,
		2,5,
		3,6
	};

	transpose(m) = m0;

	EXPECT_THAT(m(0,0), Eq(1.f));
	EXPECT_THAT(m(1,0), Eq(4.f));
	EXPECT_THAT(m(0,1), Eq(2.f));
	EXPECT_THAT(m(1,1), Eq(5.f));
	EXPECT_THAT(m(0,2), Eq(3.f));
	EXPECT_THAT(m(1,2), Eq(6.f));
}


TEST(matrix_test, transpose_transpose_access)
{
	mat2x3 m
	{
		1,3,5,
		2,4,6
	};

	transpose(transpose(m))(0, 1) = 42.f;

	EXPECT_THAT(m(0,0), Eq(1.f));
	EXPECT_THAT(m(1,0), Eq(2.f));
	EXPECT_THAT(m(0,1), Eq(42.f));
	EXPECT_THAT(m(1,1), Eq(4.f));
	EXPECT_THAT(m(0,2), Eq(5.f));
	EXPECT_THAT(m(1,2), Eq(6.f));

}

TEST(matrix_test, vectorized_cant_not_access_with_two_index)
{
	mat2x2 m{}; 
	constexpr bool can_access_with_two_index = can_access_with_two_index_v<decltype(vectorize(m))>;
	EXPECT_FALSE(can_access_with_two_index);
}

TEST(matrix_test, vectorize_access)
{
	mat2x2 m
	{
		1,3,
		2,4
	};

	float v = vectorize(m)(2);

	EXPECT_THAT(v, 3);
}

template<typename M, typename T >
void expect_mat_equal(M&& mat, const std::initializer_list<T>& expect_values)
{
	auto v = matrix_math::vectorize(mat);

	constexpr int R = std::decay_t<M>::row_num;
	constexpr int C = std::decay_t<M>::col_num;
	matrix_math::matrix<T, R, C> exp(expect_values);
	auto v_exp = matrix_math::vectorize(exp);

	for (int i = 0; i < expect_values.size(); i++)
	{
		T act = v(i);
		T exp = v_exp(i);

		EXPECT_THAT(act, Eq(exp));
	}
}

TEST(matrix_test, transpose_vec_acess)
{
	vec2 v { 1.f,2.f };

	transpose(v)(0) = 42.f;


	EXPECT_THAT(v(0), 42.f);
}

TEST(matrix_test, get_column)
{
	mat2x2 m
	{
		1,3,
		2,4
	};

	column(m, 0)(0) = 42.f;
	column(m, 0)(1) = 42.f;

	expect_mat_equal(m, 
		{
			42.f,3.f,
			42.f,4.f
		});
}



//operations

TEST(matrix_test, add)
{
	vec2 v0{ 1.f,1.f };
	vec2 v1{ 1.f,1.f };

	vec2 act = v0 + v1;

	expect_mat_equal(act, { 2.f,2.f });

}

TEST(matrix_test, add_two_view)
{
	vec2 v0{ 1.f,1.f };
	vec2 v1{ 1.f,1.f };

	auto act = vectorize(v0) + vectorize(v1) ;

	expect_mat_equal(act, { 2.f,2.f });

}

TEST(matrix_test, add_vec_and_view)
{
	vec2 v0{ 1.f,1.f };
	vec2 v1{ 1.f,1.f };

	auto act = vectorize(v0) + v1;

	expect_mat_equal(act, { 2.f,2.f });

}

TEST(matrix_test, add_three_items)
{
	vec2 v0{ 1.f,1.f };
	vec2 v1{ 1.f,1.f };

	auto act = vectorize(v0) + v1 + vectorize(v0);

	expect_mat_equal(act, { 3.f,3.f });

}

TEST(matrix_test, add_scalar_vec)
{

	vec2 v{ 1.f,1.f };

	float s = 1.f;

	auto act = v + s;

	expect_mat_equal(act, { 2.f,2.f });

}

TEST(matrix_test, negate_vec)
{
	vec2 v{ 1.f,1.f };

	auto act = -v;

	expect_mat_equal(act, { -1.f,-1.f });
}

TEST(matrix_test, add_assian)
{
	vec2 v{ 1.f,1.f };
	vec2 v1{ 1.f,1.f };

	v += v1;

	expect_mat_equal(v, { 2.f,2.f });
}

TEST(matrix_test, view_add_assian)
{
	vec2 v{ 1.f,1.f };
	tvec2 v1{ 1.f,1.f };

	transpose(v) += v1;

	expect_mat_equal(v, { 2.f,2.f });
}


TEST(matrix_test, multiplies_mat_mat)
{
	mat2x2 m0{
		1.f,2.f,
		3.f,4.f 
	};
	mat2x2 m1{ 
		1.f,3.f,
		2.f,4.f,
	};

	mat2x2 act = m0 * m1;
	expect_mat_equal(act, 
		{ 
			5.f,11.f ,
			11.f,25.f 
		});

}

TEST(matrix_test, multiplies_mat_vec)
{
	mat2x2 m{
		1.f,2.f,
		3.f,4.f 
	};
	vec2 v{ 1.f, 2.f };

	vec2 act = m * v;
	expect_mat_equal(act, { 5.f, 11.f });

}

TEST(matrix_test, multiplies_vec_mat)
{
	mat2x2 m{
		1.f,2.f,
		3.f,4.f 
	};
	vec2 v{ 1.f, 2.f };

	tvec2 act = matrix_math::transpose(v) * m;
	expect_mat_equal(act, { 7.f, 10.f });

}

TEST(matrix_test, multiplies_vec_tvec)
{
	vec2 v0{ 1.f, 2.f };
	vec2 v1{ 3.f, 4.f };

	mat2x2 act = v0 * matrix_math::transpose(v1);
	expect_mat_equal(act, 
		{
			3.f, 4.f ,
			6.f, 8.f 
		});

}

////////////// first round


TEST(matrix_test, vec_init_with_zero)
{
	vec2 v{};

	EXPECT_THAT(v(0), Eq(0.f));
	EXPECT_THAT(v(1), Eq(0.f));
}

TEST(matrix_test, matrix_init_with_zero)
{
	mat2x2 m{};

	EXPECT_THAT(m(0,0), Eq(0.f));
	EXPECT_THAT(m(1,0), Eq(0.f));
	EXPECT_THAT(m(0,1), Eq(0.f));
	EXPECT_THAT(m(1,1), Eq(0.f));
}

TEST(matrix_test, transposed_vec_init_with_zero)
{
	tvec2 v{};

	EXPECT_THAT(v(0), Eq(0.f));
	EXPECT_THAT(v(1), Eq(0.f));

}



TEST(matrix_test, transposed_vec_init)
{
	tvec2 v{
		1.f,2.f,
	};

	expect_mat_equal(v, 
		{ 
			1.f,2.f ,
		});
}

TEST(matrix_test, constexpr_init)
{
	constexpr mat2x2 m{
		1.f,2.f,
		3.f,4.f
	};

	expect_mat_equal(m, 
		{ 
			1.f,2.f ,
			3.f,4.f 
		});
}



TEST(matrix_test, add_scalar_right)
{
	vec2 v0{ 1.f,1.f };
	vec2 act = v0 + 1.f;

	expect_mat_equal(act, { 2.f,2.f });

}

TEST(matrix_test, add_scalar_left)
{
	vec2 v0{ 1.f,1.f };
	vec2 act = 1.f + v0;

	expect_mat_equal(act, { 2.f,2.f });

}

TEST(matrix_test, minus)
{
	vec2 v0{ 2.f,2.f };
	vec2 v1{ 1.f,1.f };
	vec2 act = v0 - v1;

	expect_mat_equal(act, { 1.f,1.f });

}

TEST(matrix_test, negate)
{
	vec2 v{ 1.f,1.f };
	vec2 act = -v;

	expect_mat_equal(act, { -1.f,-1.f });

}

TEST(matrix_test, minus_scalar)
{
	vec2 v0{ 2.f,2.f };
	vec2 act = v0 - 1.f;

	expect_mat_equal(act, { 1.f,1.f });

}

TEST(matrix_test, divide_scalar)
{
	vec2 v0{ 2.f,2.f };
	vec2 act = v0 / 2.f;

	expect_mat_equal(act, { 1.f,1.f });

}

TEST(matrix_test, multiplies_scalar_right)
{
	vec2 v0{ 2.f,2.f };
	vec2 act = v0 * 2.f;

	expect_mat_equal(act, { 4.f,4.f });

}

TEST(matrix_test, multiplies_scalar_left)
{
	vec2 v0{ 2.f,2.f };
	vec2 act = 2.f * v0;

	expect_mat_equal(act, { 4.f,4.f });

}

TEST(matrix_test, tranposed)
{

	mat2x2 m{ 
		1.f, 2.f ,
		3.f, 4.f 
	};
	mat2x2 act = matrix_math::transpose(m);

	expect_mat_equal(act, 
		{ 
			1.f, 3.f ,
			2.f, 4.f ,
		}
	);
}

TEST(matrix_test, tranposed_rectanglar_mat)
{

	mat2x3 m{ 
		1.f, 2.f ,3.f,
		4.f, 5.f ,6.f
	};
	mat3x2 act = matrix_math::transpose(m);

	expect_mat_equal(act, 
		{ 
			1.f, 4.f ,
			2.f, 5.f ,
			3.f, 6.f 
		}
	);
}

TEST(matrix_test, negate_transposed)
{

	mat2x3 m{ 
		1.f, 2.f ,3.f,
		4.f, 5.f ,6.f
	};
	mat3x2 act = -matrix_math::transpose(m);

	expect_mat_equal(act, 
		{ 
			-1.f, -4.f ,
			-2.f, -5.f ,
			-3.f, -6.f 
		}
	);
}

TEST(matrix_test, negate_vectorized_transposed)
{

	mat2x3 m{ 
		1.f, 2.f ,3.f,
		4.f, 5.f ,6.f
	};

	vec6 act = -matrix_math::vectorize(matrix_math::transpose(m));
	expect_mat_equal(act, { -1.f, -2.f , -3.f, -4.f , -5.f, -6.f });

}

TEST(matrix_test, transpose_vectorized)
{

	mat2x3 m{ 
		1.f, 2.f ,3.f,
		4.f, 5.f ,6.f
	};

	tvec6 act = matrix_math::transpose(matrix_math::vectorize(m));
	expect_mat_equal(act, { 1.f, 4.f , 2.f, 5.f , 3.f, 6.f });

}

TEST(matrix_test, negate_transposed_vec)
{

	vec2 v{ 
		1.f, 2.f 
	};

	tvec2 act = -matrix_math::transpose(v);
	expect_mat_equal(act, { -1.f, -2.f   });

}



TEST(matrix_test, multiply_assign_scalar)
{
	mat2x2 m
	{ 
		1.f,2.f,
		3.f,4.f 
	};

	m *= 2.f;

	expect_mat_equal(m, 
		{
			2.f, 4.f ,
			6.f, 8.f 
		});
}

TEST(matrix_test, plus_assign_scalar)
{
	mat2x2 m
	{ 
		1.f,2.f,
		3.f,4.f 
	};

	m += 2.f;

	expect_mat_equal(m, 
		{
			3.f, 4.f ,
			5.f, 6.f 
		});
}

TEST(matrix_test, minus_assign_scalar)
{
	mat2x2 m
	{ 
		1.f,2.f,
		3.f,4.f 
	};

	m -= 2.f;

	expect_mat_equal(m, 
		{
			-1.f, 0.f ,
			1.f, 2.f 
		});
}

TEST(matrix_test, divide_assign_scalar)
{
	mat2x2 m
	{ 
		2.f,4.f,
		6.f,8.f 
	};

	m /= 2.f;

	expect_mat_equal(m, 
		{
			1.f, 2.f ,
			3.f, 4.f 
		});
}


TEST(matrix_test, assign_column)
{
	mat2x2 m
	{ 
		1.f,3.f,
		2.f,4.f 
	};

	vec2 v{ 42.f,42.f };
	column(m,0) = v;

	expect_mat_equal(m,
		{
			42.f,3.f,
			42.f,4.f
		});
}

TEST(matrix_test, vectorize_write)
{
	mat2x2 m
	{ 
		1.f,3.f,
		2.f,4.f 
	};

	vec4 v{ 42.f,42.f ,42.f,42.f };
	vectorize(m) = v;

	expect_mat_equal(m,
		{
			42.f,42.f,
			42.f,42.f
		});
}

TEST(matrix_test, transpose_write)
{
	mat2x3 m
	{ 
		1.f,2.f,3.f,
		4.f,5.f,6.f
	};

	mat3x2 m1
	{ 
		11.f,14.f,
		12.f,15.f,
		13.f,16.f
	};

	transpose(m) = m1;

	expect_mat_equal(m,
		{
			11.f,12.f,13.f,
			14.f,15.f,16.f
		});
}

TEST(matrix_test, dot_product)
{
	vec2 v
	{ 
		1.f,
		0.f
	};
	float act = transpose(v) * v;

	EXPECT_THAT(act, Eq(1.f));

}

TEST(matrix_test, length)
{
	vec2 v
	{ 
		1.f,
		0.f
	};

	EXPECT_THAT(length(v), Eq(1.f));

}
