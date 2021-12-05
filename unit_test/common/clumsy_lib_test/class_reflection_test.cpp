
#include "gmock/gmock.h"
#include "clumsy_lib/class_reflection.h"
//#include "clumsy_lib/type_list.h"


template<typename T>
struct Data_Template 
{
	T aaa=3;
};


struct My_Data
{
	ADD_TYPE_TO_GROUP_AS_VALUE(A, Data_Template, float);

};




TEST(Class_Reflection_Test, as_tuple_test)
{
	My_Data data;
	auto a = clumsy_lib::as_tuple(data);
	EXPECT_THAT(std::get<0>(a).aaa, 3);


}

