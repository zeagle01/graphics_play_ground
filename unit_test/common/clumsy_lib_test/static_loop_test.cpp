
#include "gmock/gmock.h"
#include "clumsy_lib/static_loop.h"
#include <tuple>
#include <vector>
#include <string>
#include <sstream>
#include <type_traits>

using namespace clumsy_lib;
using namespace testing;


struct Print_Type_Name
{
	template<typename T>
	static void apply(std::vector<std::string>& names)
	{
		names.push_back(std::string(typeid(T).name()));
	}

};


TEST(Static_Loop_Test, for_each_type_test)
{
	auto my_t = std::make_tuple(0, 1.f);

	std::vector<std::string> act;
	For_Each_Type<decltype(my_t)>::apply<Print_Type_Name>(act);

	std::vector<std::string> exp{ "int","float" };
	for (int i = 0; i < exp.size(); i++)
	{
		EXPECT_THAT(act[i], Eq(exp[i]));
	}
}



struct  Print_Index
{
	template<int I>
	static void apply(std::vector<std::string>& out)
	{
		std::stringstream ss;
		ss << I;
		out.push_back(ss.str());
	}

};

TEST(Static_Loop_Test, for_each_in_range_test)
{

	std::vector<std::string> act;

	For_Each_In_Range<3>::apply<Print_Index>(act);

	std::vector<std::string> exp{ "0","1","2"};

	for (int i = 0; i < exp.size(); i++)
	{
		EXPECT_THAT(act[i], Eq(exp[i]));
	}

}

struct Print_Type_Name_Enumerate
{
	template<size_t I, typename T>
	static void apply(std::vector<std::string>& names)
	{

		std::stringstream ss;
		ss << I;
		names.push_back(ss.str());
		names.push_back(std::string(typeid(T).name()));
	}

};

TEST(Static_Loop_Test, for_each_type_enumerate_test)
{
	auto my_t = std::make_tuple(0, 1.f);

	std::vector<std::string> act;
	For_Each_Type_Enumerate_With_Index_Sequence<decltype(my_t), std::make_index_sequence<2>>::apply<Print_Type_Name_Enumerate>(act);

	std::vector<std::string> exp{ "0","int","1","float" };
	for (int i = 0; i < exp.size(); i++)
	{
		EXPECT_THAT(act[i], Eq(exp[i]));
	}

}
