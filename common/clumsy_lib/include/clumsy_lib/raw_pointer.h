
#pragma once

#include <vector>

namespace clumsy_lib
{

	template<typename T>
	struct Raw_Pointer 
	{
		using element_type = void;
		void* ptr;
		int size = 0;
	};

	template<typename T>
	struct Raw_Pointer<std::vector<T>> 
	{
		using element_type = T;
		T* ptr;
		int size;
	};

}
