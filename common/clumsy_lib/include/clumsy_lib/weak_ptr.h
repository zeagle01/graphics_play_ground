
#pragma once

#include <memory>

#define SAFE_INVOKE(f,...) invoke([&](auto o) {return o->f(__VA_ARGS__); })

namespace clumsy_lib
{
	template<typename T>
	class Weak_Ptr
	{
	public:
		Weak_Ptr(std::shared_ptr<T> obj) :m_data(obj) {}

		template<typename F>
		decltype(auto) invoke(F&& f)
		{
			if (auto temp = m_data.lock())
			{
				return f(temp);
			}
			else
			{
				using ret_type = decltype(f(std::declval<std::shared_ptr<T>>()));
				return ret_type{};
			}
		};

	private:
		std::weak_ptr<T> m_data;
	};

}

