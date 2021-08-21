
#include "gmock/gmock.h"
#include "clumsy_lib/type_map.h"
#include "clumsy_lib/dependent_variable_set.h"
#include "clumsy_lib/class_reflection.h"
#include <memory>
#include <string>


using namespace testing;
using namespace clumsy_lib;


template<typename T>
class Data_Holder
{
private:
	T data;
public:
	Data_Holder() = default;

	using value_type = T;

	void set(const T& d)
	{
		data = d;
	}
	const T& get() const
	{
		return data;
	}

	T& get_ref()
	{
		return data;
	}
};


struct Base 
{
	virtual ~Base() {}
};


struct Var_A :public Base, Data_Holder<int>
{
};
struct Var_B :Base {};
struct Var_C :Base {};



struct Data_Group
{
	ADD_TYPE_TO_GROUP(A, Data_Holder, int);
	ADD_TYPE_TO_GROUP(B, Data_Holder, float);
	ADD_TYPE_TO_GROUP(C, Data_Holder, double);
};




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

TEST(Type_Map_Test,type_map_add_types_with_group)
{
	Type_Map map;

	map.add_types<Data_Group>();
	auto act_type = map.get_type<Data_Group::A>();

	auto act= std::string( typeid(*act_type.get()).name());
	std::string exp = "class Data_Group::A";

	EXPECT_THAT(act, Eq(exp));

}

TEST(Type_Map_Test,type_map_set_value)
{
	auto variable_set = std::make_shared<Variable_Set>();

	variable_set->add_type<Var_A>();

	variable_set->set_value<Var_A>(10);
	auto act = variable_set->get_value<Var_A>();
	int exp = 10;

	EXPECT_THAT(act, Eq(exp));
}

///////////////////////////////

template<typename value_type>
struct Variable_Record :Base, Data_Holder<value_type> {};

#define DEF_MEM(x,t) ADD_TYPE_TO_GROUP(x,Variable_Record,t)

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
	auto data_accecor = std::make_shared<v_a_t>();
	data_accecor->set_type_map(varialbe_set);

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

struct Compute_Acceleration
{
	template<typename data_acc, typename T>
	static void apply(data_acc& datas, T& d)
	{
		auto f = datas.get_value<dependent_variables_struct::Force>();
		d = f * 0.5f;
	}
};


#define DEF_DEPENDENT_MEM(x,value_t,computer,dependent_list) ADD_TYPE_TO_GROUP(x,Dependent_Variable,value_t,computer,dependent_list)

struct dependent_variables_struct
{
	DEF_DEPENDENT_MEM(Position, float, Plain_Computer, CE_WRAP(type_list<>));
	DEF_DEPENDENT_MEM(Velocity, float, Plain_Computer, CE_WRAP(type_list<>));
	DEF_DEPENDENT_MEM(Force, float, Compute_Force, CE_WRAP(type_list<Velocity, Position>));
	DEF_DEPENDENT_MEM(Acceleration, float, Compute_Acceleration, CE_WRAP(type_list<Force >));
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
	auto a = varialbe_set->get_value<dependent_variables_struct::Acceleration>();

	EXPECT_THAT(f, Eq(3.f));
	EXPECT_THAT(a, Eq(1.5f));


	///////force set last
	varialbe_set->set_value<dependent_variables_struct::Position>(1.f);
	varialbe_set->set_value<dependent_variables_struct::Velocity>(2.f);
	varialbe_set->set_value<dependent_variables_struct::Force>(4.f);

	p = varialbe_set->get_value<dependent_variables_struct::Position>();
	v = varialbe_set->get_value<dependent_variables_struct::Velocity>();
	f = varialbe_set->get_value<dependent_variables_struct::Force>();
	a = varialbe_set->get_value<dependent_variables_struct::Acceleration>();

	EXPECT_THAT(f, Eq(4.f));
	EXPECT_THAT(a, Eq(2.f));

	///////force set first
	varialbe_set->set_value<dependent_variables_struct::Force>(4.f);
	varialbe_set->set_value<dependent_variables_struct::Position>(1.f);
	varialbe_set->set_value<dependent_variables_struct::Velocity>(2.f);

	p = varialbe_set->get_value<dependent_variables_struct::Position>();
	v = varialbe_set->get_value<dependent_variables_struct::Velocity>();
	f = varialbe_set->get_value<dependent_variables_struct::Force>();
	a = varialbe_set->get_value<dependent_variables_struct::Acceleration>();

	EXPECT_THAT(f, Eq(4.f));
	EXPECT_THAT(a, Eq(2.f));

}
