#pragma once

#include <type_traits>

namespace clumsy_engine
{

	struct anyT
	{
		size_t dummy;
		template <typename T>
		operator T& ();

		template <typename T>
		operator T* () { return nullptr; }
	};

	template <typename T, typename I0 = void, size_t... I>
	struct field_count
	{
		static constexpr size_t value = sizeof...(I) - 1;
	};

	template <typename T, size_t... I>
	struct field_count < T, std::void_t< decltype(T{ anyT{I}... }) > , I... >
	{
		static constexpr size_t value = field_count<T, void, sizeof...(I), I... >::value;
	};

	template <typename T>
	constexpr auto as_tuple(T&& val) noexcept
	{

		constexpr auto count = field_count<T>::value;

		if constexpr (count == 1)
		{

			auto& [a] = std::forward<T>(val);

			return std::make_tuple(a);
		}
		else if constexpr (count == 2)
		{
			auto& [a, b] = std::forward<T>(val);
			return std::make_tuple(a, b);
		}
		else if constexpr (count == 3)
		{
			auto& [a, b, c] = std::forward<T>(val);
			return std::make_tuple(a, b, c);
		}
		else if constexpr (count == 3)
		{
			auto& [a, b, c] = std::forward<T>(val);
			return std::make_tuple(a, b, c);
		}
		else if constexpr (count == 4)
		{
			auto& [a, b, c, d] = std::forward<T>(val);
			return std::make_tuple(a, b, c, d);
		}
		else if constexpr (count == 5)
		{
			auto& [a, b, c, d, e] = std::forward<T>(val);
			return std::make_tuple(a, b, c, d, e);
		}
		else if constexpr (count == 6)
		{
			auto& [a, b, c, d, e, f] = std::forward<T>(val);
			return std::make_tuple(a, b, c, d, e, f);
		}
	}




}
