
#include "gmock/gmock.h"
#include "clumsy_lib/type_map.h"
#include "clumsy_lib/class_reflection.h"
#include <memory>
#include <string>


using namespace testing;
using namespace clumsy_lib;

struct Base 
{
	virtual ~Base() {}
};


struct Var_A :public Base, Data_Holder<int>
{
};
struct Var_B :Base {};
struct Var_C :Base {};


TEST(Type_Map_Test,type_map_add_with_object)
{
	Type_Map<Base> map;

	std::shared_ptr<Var_A> va = std::make_shared<Var_A>();

	map.add(va);

	auto act = map.get<Var_A>();

	EXPECT_THAT(act, Eq(va));
}

TEST(Type_Map_Test,type_map_add_with_type)
{
	Type_Map<Base> map;

	map.add<Var_A>();
	auto act_type = map.get<Var_A>();

	auto act= std::string( typeid(*act_type.get()).name());
	std::string exp = "struct Var_A";

	EXPECT_THAT(act, Eq(exp));

}

TEST(Type_Map_Test,type_map_set_value)
{
	Variable_Set<Base> variable_set;

	variable_set.add_variable<Var_A>();

	variable_set.set_value<Var_A>(10);
	auto act = variable_set.get_value<Var_A>();
	int exp = 10;

	EXPECT_THAT(act, Eq(exp));
}

///////////////////////////////

template<typename value_type>
struct Variable_Record :Base, Data_Holder<value_type> {};

#define DEF_MEM(x,t) DEF_DATA_CLASS_MEM(x,Variable_Record,t)

struct my_variables_struct
{
	DEF_MEM(Position, float);
	DEF_MEM(Velocity, float);
};




TEST(Type_Map_Test,build_variable_set)
{
	auto variable_set = build_variable_set<my_variables_struct, Base>();

	auto act = variable_set->get_value<my_variables_struct::Position>();

}
