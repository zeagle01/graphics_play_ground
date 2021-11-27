


#include "matrix_math/matrix_math.h"
#include "gmock/gmock.h"


using namespace testing;



//TEST(Matrix_Test, acess_by_element)
//{
//	mat<2, 2, float> m
//	{
//		{0,1},
//		{2,3}
//	};
//
//	EXPECT_THAT(m(0,0), Eq(0));
//	EXPECT_THAT(m(1,0), Eq(1));
//	EXPECT_THAT(m(0,1), Eq(2));
//	EXPECT_THAT(m(1,1), Eq(3));
//}
//
//TEST(Matrix_Test, assign_by_element)
//{
//	mat<2, 2, float> m
//	{
//		{0,1},
//		{2,3}
//	};
//	m(0,0) = 5;
//
//	EXPECT_THAT(m(0,0), Eq(5));
//}
//
//TEST(Matrix_Test, get_column)
//{
//	mat<2, 2, float> m
//	{
//		{0,1},
//		{2,3}
//	};
//	vec<2,float> act = m.column(0);
//	vec<2, float> exp{ 0,1 };
//
//	EXPECT_TRUE(is_near<0>(act, exp));
//}
//
//TEST(Vector_Test, get_element)
//{
//	vec<2,  float> v{0,1};
//
//	EXPECT_THAT(v(0), Eq(0));
//	EXPECT_THAT(v(1), Eq(1));
//}
//
//TEST(Matrix_Test, assign_construct)
//{
//	mat<2, 2, float> m
//	{
//		{0,1},
//		{2,3}
//	};
//	mat<2, 2, float> m1(m);
//
//	EXPECT_TRUE(is_near<0>(m1, m));
//
//}
//
//TEST(Vector_Test, assign_construct)
//{
//	vec2f v{0,1};
//	auto v1 = v;
//
//	EXPECT_TRUE(is_near<0>(v1, v));
//}
//
//
//
//
///// add
//TEST(Matrix_Test, add)
//{
//	mat2x2f m0 = get_uniform<2, 2, float>(1);
//	mat2x2f m1 = get_uniform<2, 2, float>(1);
//
//	auto act = m0 + m1;
//	mat2x2f exp = { {2,2},{2,2} };
//
//	EXPECT_TRUE(is_near<0>(act, exp, 1e-6f));
//}
//
//TEST(Vector_Test, add)
//{
//	vec2f v0 = get_uniform<2, 1, float>(1);
//	vec2f v1 = get_uniform<2, 1, float>(1);
//
//	auto act = v0 + v1;
//
//	vec2f exp = { 2,2 };
//
//	EXPECT_TRUE(is_near<0>(act, exp, 1e-6f));
//}
//
//
///// scalar multiplication
//TEST(Vector_Test, scalar_multiplication)
//{
//	vec2f v0{1,1};
//
//	auto exp = v0 * 2.f;
//	auto exp1 = 2.f * v0;
//	EXPECT_THAT(exp(0), Eq(2));
//	EXPECT_THAT(exp(1), Eq(2));
//	EXPECT_THAT(exp1(0), Eq(2));
//	EXPECT_THAT(exp1(1), Eq(2));
//}
//
///// norm
//TEST(Vector_Test, norm)
//{
//	vec2f v0{1,1};
//	vec2f v1{3,4};
//
//	float norm1 = Vectorized_Norm<1>()(v0); //1+1
//	float norm2 = Vectorized_Norm<2>()(v1); // sqrt(3*3+4*4)=5
//	float norm3 = Vectorized_Norm<0>()(v1); //infinity norm
//	float norm4 = Vectorized_Norm<1>()(v1 - v0); //2+3
//	EXPECT_THAT(norm1, Eq(2));
//	EXPECT_THAT(norm2, Eq(5));
//	EXPECT_THAT(norm3, Eq(4));
//	EXPECT_THAT(norm4, Eq(5));
//}
//
//TEST(Matrix_Test, norm)
//{
//	mat2x2f m0 = get_uniform<2, 2, float>(1);
//	float exp = Vectorized_Norm<1>()(m0);
//	EXPECT_THAT(exp, Eq(4));
//}
//
//
///// is_near
//TEST(Vector_Test, is_near)
//{
//	vec2f v0{1,1};
//	vec2f v1{1,1};
//	vec2f v2{1,1.1};
//
//	auto exp1 = is_near<1>(v1, v0);
//	auto exp2 = is_near<1>(v2, v0);
//	auto exp3 = is_near<1>(v2, v0, 0.11f);
//	EXPECT_TRUE(exp1);
//	EXPECT_FALSE(exp2);
//	EXPECT_TRUE(exp3);
//}
//
///// vv dot
//TEST(Vector_Test, dot)
//{
//	vec2f v0{1,1};
//	vec2f v1{1,-1};
//
//	float exp = transpose(v0) & v1;
//	EXPECT_THAT(exp,Eq(0));
//}
//
///// mv dot
//TEST(Matrix_Test, dot)
//{
//	mat2x2f m0
//	{
//		{0,1},
//		{2,3}
//	};
//
//	mat2x2f m1
//	{
//		{1,1},
//		{1,1}
//	};
//	vec2f v{1,-1};
//
//	auto exp0 = m0&v;
//
//	EXPECT_THAT(exp0(0),Eq(-2));
//	EXPECT_THAT(exp0(1),Eq(-2));
//	EXPECT_TRUE(is_near<0>(m0 & v , v & m0));
//
//	auto exp1 = m0 & m1;
//	EXPECT_THAT(exp1(0,0),Eq(2));
//	EXPECT_THAT(exp1(1,0),Eq(4));
//	EXPECT_THAT(exp1(0,1),Eq(2));
//	EXPECT_THAT(exp1(1,1),Eq(4));
//}
//
///// cross
//TEST(Vector_Test, cross)
//{
//	vec3f v0{1,0,0};
//	vec3f v1{0,1,0};
//
//	auto exp = cross(v0, v1);
//	EXPECT_THAT(exp(0),Eq(0));
//	EXPECT_THAT(exp(1),Eq(0));
//	EXPECT_THAT(exp(2),Eq(1));
//}
//
//
//
//
///////////// 
//TEST(Matrix_Test, determinant_2by2)
//{
//	mat2x2f A{ {1,2},{3,4} };
//
//	auto act = determinant<2, float>::get(A);
//
//	float exp = -2.f;
//
//	EXPECT_THAT(act, Eq(exp));
//}
//
//
//TEST(Matrix_Test, solve)
//{
//	mat3x3f A = get_identity<3, float>();
//
//	vec3f b{ 0,1,0 };
//	vec3f exp{ 0,1,0 };
//
//	auto act = b|A;
//
//	EXPECT_THAT(act(0), Eq(exp(0)));
//	EXPECT_THAT(act(1), Eq(exp(1)));
//	EXPECT_THAT(act(2), Eq(exp(2)));
//}
//
//TEST(Matrix_Test, solve_2112)
//{
//	mat2x2f A{ {2,1},{1,2} };
//
//	vec2f b{ 3,3, };
//	vec2f exp{ 1,1 };
//
//	auto act = b | A;
//
//	EXPECT_THAT(act(0), Eq(exp(0)));
//	EXPECT_THAT(act(1), Eq(exp(1)));
//}
//
//
//TEST(Matrix_Test, normalized_vec)
//{
//
//	vec2f v{ 3,0 };
//
//	vec2f act = normalize(v);
//	vec2f exp = { 1,0 };
//
//
//	EXPECT_THAT(act(0), Eq(exp(0)));
//	EXPECT_THAT(act(1), Eq(exp(1)));
//
//	v={ 0,0 };
//	act = normalize(v);
//	exp = { 0,0 };
//
//	EXPECT_THAT(act(0), Eq(exp(0)));
//	EXPECT_THAT(act(1), Eq(exp(1)));
//}
//

namespace matrix_math
{
	TEST(Matrix_Math_Test, init)
	{
		mat< 2, 1, float> act{ 1,2 };

		EXPECT_THAT(act(0, 0), Eq(1));
		EXPECT_THAT(act(1, 0), Eq(2));

		EXPECT_THAT(act(0), Eq(1));
		EXPECT_THAT(act(1), Eq(2));
	}

	TEST(Matrix_Math_Test, add_minus)
	{
		mat< 2, 1, float> a{ 1,2 };
		mat< 2, 1, float> b{ 1,2 };

		auto act = a + b;

		EXPECT_THAT(act(0), Eq(2));
		EXPECT_THAT(act(1), Eq(4));

		act = a - b;

		EXPECT_THAT(act(0), Eq(0));
		EXPECT_THAT(act(1), Eq(0));
	}

	TEST(Matrix_Math_Test, self_add)
	{
		mat< 2, 1, float> a{ 1,2 };
		mat< 2, 1, float> b{ 1,2 };

		auto act = a ;

		act += b;
		EXPECT_THAT(act(0), Eq(2));
		EXPECT_THAT(act(1), Eq(4));

		act -=  b;
		EXPECT_THAT(act(0), Eq(1));
		EXPECT_THAT(act(1), Eq(2));
	}

	TEST(Matrix_Math_Test, matrix_multiply_scalar)
	{
		mat< 2, 1, float> a{ 1,2 };

		auto act = a * 2.f;

		EXPECT_THAT(act(0), Eq(2));
		EXPECT_THAT(act(1), Eq(4));

		act = 3.f * a;

		EXPECT_THAT(act(0), Eq(3));
		EXPECT_THAT(act(1), Eq(6));

		act = act / 3.f;

		EXPECT_THAT(act(0), Eq(1));
		EXPECT_THAT(act(1), Eq(2));
	}

	TEST(Matrix_Math_Test, matrix_multiply)
	{
		mat< 2, 1, float> a{ 1,2 };
		mat< 1, 2, float> b{ 1,2 };

		auto act = a * b;

		EXPECT_THAT(act(0,0), Eq(1));
		EXPECT_THAT(act(1,0), Eq(2));
		EXPECT_THAT(act(0,1), Eq(2));
		EXPECT_THAT(act(1,1), Eq(4));

		auto act1 = b * a;

		EXPECT_THAT(act1, Eq(5));
	}

	TEST(Matrix_Math_Test, cross)
	{
		vec3 v0{ 1,0,0 };
		vec3 v1{ 0,1,0 };

		auto exp = v0 ^ v1;
		EXPECT_THAT(exp(0), Eq(0));
		EXPECT_THAT(exp(1), Eq(0));
		EXPECT_THAT(exp(2), Eq(1));
	}

	TEST(Matrix_Math_Test, solve)
	{
		mat3 A = get_identity<3, float>();

		vec3 b{ 0,1,0 };
		vec3 exp{ 0,1,0 };

		auto act = b / A;

		EXPECT_THAT(act(0), Eq(exp(0)));
		EXPECT_THAT(act(1), Eq(exp(1)));
		EXPECT_THAT(act(2), Eq(exp(2)));
	}

	TEST(Matrix_Math_Test, vectorize_test)
	{
		auto A = get_identity<2, float>();


		auto act = vectorize(A);
	}

	TEST(Matrix_Math_Test, norm_test)
	{
		auto A = get_identity<2, float>();

		auto act = norm<2>::apply(A);

		EXPECT_THAT(act, Eq(sqrt(2.f)));
	}

	TEST(Matrix_Math_Test, is_near_test)
	{
		vec3 v0{ 1,0,0 };
		vec3 v1{ 1,0,0 };


		EXPECT_TRUE(is_near<2>(v0, v1));

		 v0(0)+=0.0001;

		EXPECT_FALSE(is_near<2>(v0, v1));
	}
}
