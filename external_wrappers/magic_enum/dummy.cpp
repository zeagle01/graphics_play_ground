

#include "magic_enum/magic_enum.hpp"

#include <string>

enum some_enum
{
	A,
	B
};


constexpr auto get_enum_type_name()
{
	return magic_enum::enum_type_name<some_enum>();
}