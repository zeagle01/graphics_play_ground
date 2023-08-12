

#pragma once
#include <memory>


namespace clumsy_engine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope_Ref = std::unique_ptr<T>;
}
