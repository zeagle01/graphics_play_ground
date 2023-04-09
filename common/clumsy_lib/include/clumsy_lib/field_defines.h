

#pragma once

#define ADD_TAG(name) struct name{}; name*  name##_var;
#define ADD_FIELD(name,t) struct name{ using type=t;}; name*  name##_var;
#define ADDD_FIELD_WITH(name,t,...) struct name{ using type=t; __VA_ARGS__}; name*  name##_var;


template<typename T>
struct Get_Its_Type
{
	using type = typename T::type;
};
