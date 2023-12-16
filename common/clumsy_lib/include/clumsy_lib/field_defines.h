

#pragma once


#define CE_ADD_NODE(name,...) struct name{__VA_ARGS__;}; name*  name##_var;
#define CE_FIELD(name,...) using name=__VA_ARGS__;

#define CE_ADD_LEAF(name,...) struct name{__VA_ARGS__;}; 
#define CE_TYPE(t) CE_FIELD(type,t)


#define CE_ADD_FIELD(name,t) CE_ADD_NODE(name,CE_TYPE(t) )


#define CE_ENTRY(name,...) struct name{__VA_ARGS__;}; name*  name##_var
#define CE_USE(name,...) using name=__VA_ARGS__
#define CE_LIST(name,...) using name=clumsy_lib::type_list<__VA_ARGS__>
#define CE_VAL(name,...) CE_USE(name,clumsy_lib::literal_t<__VA_ARGS__>)
#define CE_NIL(name) CE_USE(name,nullptr_t)