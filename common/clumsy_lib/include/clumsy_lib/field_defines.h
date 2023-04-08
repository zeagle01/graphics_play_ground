

#pragma once

#define ADD_FIELD(name,t) struct name{ using type=t;}; name*  name##_var;
