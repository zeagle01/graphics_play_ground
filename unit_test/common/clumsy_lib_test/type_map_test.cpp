
#include "gmock/gmock.h"
#include "clumsy_lib/type_map.h"
#include "clumsy_lib/variable_set.h"
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
	Type_Map map;

	std::shared_ptr<Var_A> va = std::make_shared<Var_A>();

	map.add_type(va);

	auto act = map.get_type<Var_A>();

	EXPECT_THAT(act, Eq(va));
}

TEST(Type_Map_Test,type_map_add_with_type)
{
	Type_Map map;

	map.add_type<Var_A>();
	auto act_type = map.get_type<Var_A>();

	auto act= std::string( typeid(*act_type.get()).name());
	std::string exp = "struct Var_A";

	EXPECT_THAT(act, Eq(exp));

}

TEST(Type_Map_Test,type_map_set_value)
{
	Variable_Set variable_set;

	variable_set.add_type<Var_A>();

	variable_set.set_value<Var_A>(10);
	auto act = variable_set.get_value<Var_A>();
	int exp = 10;

	EXPECT_THAT(act, Eq(exp));
}

///////////////////////////////

template<typename value_type>
struct Variable_Record :Base, Data_Holder<value_type> {};

#define DEF_MEM(x,t) DEF_DATA_CLASS_MEM(x,Variable_Record,t)

struct variables_struct
{
	DEF_MEM(Position, float);
	DEF_MEM(Velocity, float);
};




TEST(Type_Map_Test,build_variable_set)
{
	auto variable_set = Variable_Set::build_variable_set<variables_struct >();

	variable_set->set_value < variables_struct::Position>(1.f);
	auto act = variable_set->get_value<variables_struct::Position>();
	float exp = 1.f;

	EXPECT_THAT(act, Eq(exp));

}

TEST(Type_Map_Test, data_accecor_test)
{
	//using v_s_t = Variable_Set;
	auto varialbe_set = Variable_Set::build_variable_set< variables_struct >();

	using v_a_t = Variable_Accecor_With_Constriant<Variable_Set, type_list<variables_struct::Velocity>>;
	auto data_accecor = std::make_shared<v_a_t>(varialbe_set.get());

	//data_accecor->set_value<variables_struct::Position>(1.f);//constranit violation, compile error

	data_accecor->set_value<variables_struct::Velocity>(1.f);
	auto act = data_accecor->get_value<variables_struct::Velocity>();
	float exp = 1.f;

	EXPECT_THAT(act, Eq(exp));

}


///////////////////////////////

struct dependent_variables_struct;

struct Compute_Force
{
	template<typename data_acc, typename T>
	static void apply(data_acc& datas, T& d)
	{
		auto p = datas.get_value<dependent_variables_struct::Position>();
		auto v = datas.get_value<dependent_variables_struct::Velocity>();
		//auto f = datas.get_value<dependent_variables_struct::Force>();
		d = p + v;
	}
};


#define DEF_DEPENDENT_MEM(x,value_t,computer,dependent_list) DEF_DATA_CLASS_MEM(x,Dependent_Variable,value_t,computer,dependent_list)

struct dependent_variables_struct
{
	DEF_DEPENDENT_MEM(Position, float, Plain_Computer, TMP(type_list<>));
	DEF_DEPENDENT_MEM(Velocity, float, Plain_Computer, TMP(type_list<>));
	DEF_DEPENDENT_MEM(Force, float, Compute_Force, TMP(type_list<Velocity, Position>));
};



TEST(Type_Map_Test, dependent_data_accecor_test)
{
	auto varialbe_set = build_dependent_variable_set<Variable_Set, dependent_variables_struct >();

	///////force will be computed automaticly
	varialbe_set->set_value<dependent_variables_struct::Position>(1.f);
	varialbe_set->set_value<dependent_variables_struct::Velocity>(2.f);

	auto p = varialbe_set->get_value<dependent_variables_struct::Position>();
	auto v = varialbe_set->get_value<dependent_variables_struct::Velocity>();
	auto f = varialbe_set->get_value<dependent_variables_struct::Force>();

	EXPECT_THAT(f, Eq(3.f));


	///////force set last
	varialbe_set->set_value<dependent_variables_struct::Position>(1.f);
	varialbe_set->set_value<dependent_variables_struct::Velocity>(2.f);
	varialbe_set->set_value<dependent_variables_struct::Force>(4.f);//will be ignore

	p = varialbe_set->get_value<dependent_variables_struct::Position>();
	v = varialbe_set->get_value<dependent_variables_struct::Velocity>();
	f = varialbe_set->get_value<dependent_variables_struct::Force>();

	EXPECT_THAT(f, Eq(4.f));

	///////force set first
	varialbe_set->set_value<dependent_variables_struct::Force>(4.f);//will be ignore
	varialbe_set->set_value<dependent_variables_struct::Position>(1.f);
	varialbe_set->set_value<dependent_variables_struct::Velocity>(2.f);

	p = varialbe_set->get_value<dependent_variables_struct::Position>();
	v = varialbe_set->get_value<dependent_variables_struct::Velocity>();
	f = varialbe_set->get_value<dependent_variables_struct::Force>();

	EXPECT_THAT(f, Eq(4.f));

}
