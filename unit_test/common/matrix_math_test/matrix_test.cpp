
#include "gmock/gmock.h"
#include <initializer_list>

using namespace testing;


import matrix_math;


using mat2x2 = matrix_math::matrix<float, 2, 2>;
using mat2x3 = matrix_math::matrix<float, 2, 3>;
using mat3x2 = matrix_math::matrix<float, 3, 2>;
using vec2 = matrix_math::matrix<float, 2, 1>;
using tvec2 = matrix_math::matrix<float, 1, 2>;
using vec4 = matrix_math::matrix<float, 4, 1>;
using vec6 = matrix_math::matrix<float, 6, 1>;
using tvec6 = matrix_math::matrix<float, 1, 6>;


template<typename T,int R, int C>
void expect_mat_equal(const matrix_math::matrix<T, R, C>& mat, const std::initializer_list<T>& expect_values)
{
	auto v = matrix_math::vectorize(mat);
	matrix_math::matrix<T, R, C> exp(expect_values);
	auto v_exp = matrix_math::vectorize(exp);

	for (int i = 0; i < v.size(); i++)
	{
		T act = v(i);
		T exp = v_exp(i);

		EXPECT_THAT(act, Eq(exp));
	}
}



TEST(matrix_test, init_with_one_value)
{
	vec2 m(1.f);

	expect_mat_equal(m, { 1.f,1.f });
}

TEST(matrix_test, init_with_values)
{
	mat2x2 m{
		1.f,2.f,
		3.f,4.f
	};

	expect_mat_equal(m, 
		{ 
			1.f,2.f ,
			3.f,4.f 
		});
}

TEST(matrix_test, transposed_init)
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

TEST(matrix_test, vectorize)
{
	constexpr mat2x2 m{
		1.f,2.f,
		3.f,4.f
	};

	vec4 v = vectorize(m);

	EXPECT_THAT(v(0), Eq(m(0,0)));
	EXPECT_THAT(v(1), Eq(m(1,0)));
	EXPECT_THAT(v(2), Eq(m(0,1)));
	EXPECT_THAT(v(3), Eq(m(1,1)));

}

TEST(matrix_test, add)
{
	vec2 v0{ 1.f,1.f };
	vec2 v1{ 1.f,1.f };
	vec2 act = v0 + v1;

	expect_mat_equal(act, { 2.f,2.f });

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
