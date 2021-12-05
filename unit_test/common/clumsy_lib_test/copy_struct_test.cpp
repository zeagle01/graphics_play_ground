
#include "gmock/gmock.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/type_list.h"

struct Ptr_Base {};

template<typename T>
struct Ptr :Ptr_Base { T* data; };


#define MEMBER_NAME(x) x##_var

#define GET_MEMBER(obj,T) obj.MEMBER_NAME(T) 

struct Param_Set
{
	ADD_TYPE_TO_GROUP(A, Ptr, float);
	ADD_TYPE_TO_GROUP(B, Ptr, float);
	ADD_TYPE_TO_GROUP(C, Ptr, float);
};


template <typename T>
struct copy_
{
	template <typename It>
	static void apply(T& a, const T& b)
	{
		//GET_MEMBER(a, It) = GET_MEMBER(b, It);
	}
};


template <typename T>
struct copy_struct
{
	static void apply(T& a, const T& b)
	{
		using member_list = clumsy_lib::extract_member_type_list_t<T>;
		clumsy_lib::for_each_type<member_list, copy_<T> >(a, b);
	}
};

TEST(Copy_Struct_Test,copy_test)
{

	Param_Set a;
	Param_Set b;
	
	std::vector<float> a_arr(2, 1);
	std::vector<float> b_arr(2, 1);
	std::vector<float> c_arr(2, 1);

	//a.A_var->data = a_arr.data();
	//a.B_var->data = b_arr.data();
	//a.C_var->data = c_arr.data();

	//copy_struct<Param_Set>::apply(b, a);

	//for_each_type
}
