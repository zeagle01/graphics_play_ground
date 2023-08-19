

#pragma once


#define CE_ADD_NODE(name,...) struct name{__VA_ARGS__}; name*  name##_var;
#define CE_ADD_LEAF(name,...) struct name{__VA_ARGS__}; 

#define CE_ADD_TAG(name) struct name{}; 
#define CE_FIELD(name,t) using name=t;
#define CE_TYPE(t) CE_FIELD(type,t)



#define CE_ADD_FIELD(name,t) CE_ADD_NODE(name,CE_TYPE(t) )
