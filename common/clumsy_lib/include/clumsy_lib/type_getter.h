
#pragma once

namespace clumsy_lib
{
	template<typename T>
	struct Get_Its_Own_Type
	{
		using type = typename T::type;
	};

	template<typename T>
	struct Get_Its_Own_As_Type
	{
		using type = T;
	};

}
