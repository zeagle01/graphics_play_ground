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

			auto [v0] = std::forward<T>(val);

			return std::make_tuple(v0);
		}
		else if constexpr (count == 2)
		{
			auto [v0, v1] = std::forward<T>(val);
			return std::make_tuple(v0, v1);
		}
		else if constexpr (count == 3)
		{
			auto [v0, v1, v2] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2);
		}
		else if constexpr (count == 4)
		{
			auto [v0, v1, v2,v3] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3);
		}
		else if constexpr (count == 5)
		{
			auto [v0, v1, v2, v3,v4] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4);
		}
		else if constexpr (count == 6)
		{
			auto [v0, v1, v2, v3, v4,v5] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5);
		}
		else if constexpr (count == 7)
		{
			auto [v0, v1, v2, v3, v4,v5,v6] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6);
		}
		else if constexpr (count == 8)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7);
		}
		else if constexpr (count == 9)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7,v8] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7,v8);
		}
		else if constexpr (count == 10)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7,v8,v9] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7,v8,v9);
		}
		else if constexpr (count == 11)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7,v8,v9,v10] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7,v8,v9,v10);
		}
		else if constexpr (count == 12)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7,v8,v9,v10,v11] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7,v8,v9,v10,v11);
		}
		else if constexpr (count == 13)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7,v8,v9,v10,v11,v12] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7,v8,v9,v10,v11,v12);
		}
		else if constexpr (count == 14)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7,v8,v9,v10,v11,v12,v13] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7,v8,v9,v10,v11,v12,v13);
		}
		else if constexpr (count == 15)
		{
			auto [v0, v1, v2, v3, v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14] = std::forward<T>(val);
			return std::make_tuple(v0, v1, v2, v3, v4, v5,v6,v7,v8,v9,v10,v11,v12,v13,v14);
		}

	}




}
