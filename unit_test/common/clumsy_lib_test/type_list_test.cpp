
#include "gmock/gmock.h"
#include "clumsy_lib/type_list.h"


using namespace testing;
using namespace clumsy_lib;

TEST(Type_List_Test, get_front_test)
{
	using tl = type_list<int, float>;

	using act = front_t<tl>;
	using exp = int;

	bool ret = std::is_same_v<act, exp>;
	EXPECT_TRUE(ret);
}

TEST(Type_List_Test, pop_front_test)
{

	using tl = type_list<int, float>;

	using act = pop_front_t<tl>;

	using exp = type_list<float>;

	bool ret = std::is_same_v<act, exp>;
	EXPECT_TRUE(ret);
}


TEST(Type_List_Test, push_front_test)
{

	using tl = type_list<int >;

	using act = push_front_t<tl, double>;

	using exp = type_list<double, int >;

	bool ret = std::is_same_v<act, exp>;

	EXPECT_TRUE(ret);
}

TEST(Type_List_Test, get_2nd_type)
{

	using tl = type_list<int, float>;

	using act = get_nth_element_t< tl, 1>;

	using exp = float;

	bool ret = std::is_same_v<act, exp>;

	EXPECT_TRUE(ret);
}


TEST(Type_List_Test, empty_list_is_empty)
{

	EXPECT_TRUE(is_empty_v<type_list<>>);

	EXPECT_FALSE(is_empty_v<type_list<int>>);
}



TEST(Type_List_Test, get_type_list_size)
{

	EXPECT_THAT(get_size_v<type_list<>>, Eq(0));

	EXPECT_THAT(get_size_v<type_list<int>>, Eq(1));
}

TEST(Type_List_Test, get_type_index)
{
	auto aa = get_index_v < type_list<>, int >;
	auto bb = get_index_v < type_list<int>, int >;
	auto cc = get_index_v < type_list<float, int>, int >;
	
	EXPECT_THAT(aa, Eq(-1));
	EXPECT_THAT(bb, Eq(0));
	EXPECT_THAT(cc, Eq(1));
}
