
#pragma once

#include <type_traits>
#include <tuple>
#include "macro_loop.h"
#include "type_list.h"

namespace soft_render
{

	namespace details
	{
		//get field count
		struct to_any
		{
			template<typename T>
			operator T();

			template<typename T>
			operator T* ();
		};

		template<typename T, typename U = void, typename ...P>
		struct get_field_count
		{
			static constexpr int value = sizeof...(P) - 1;
		};

		template<typename T, typename ...P>
		struct get_field_count < T, std::void_t < decltype(T{ P{}... }) > , P... >
		{
			static constexpr int value = get_field_count<T, void, to_any, P...>::value;
		};


		//as tuple
#define VV_B(n) v##n
#define VV(n) ,v##n

#define VAR_LIST(n) INSIDE_LOOP(n,VV_B,VV)


#define IF_BRANCH(num)\
if constexpr (get_field_count<T>::value ==0){ \
		return std::make_tuple(); \
}\

#define ELSE_IF_BRANCH(num)\
else if constexpr (get_field_count<T>::value ==num){ \
		auto [VAR_LIST(num)] = T{}; \
		return std::make_tuple(VAR_LIST(num)); \
}\


#define TOTUPLES(n) LOOP(n,IF_BRANCH,ELSE_IF_BRANCH)


		template<typename T>
		constexpr auto as_tuple()
		{
			EVAL(TOTUPLES(32));
		}


		////////from exsiting object
#define IF_BRANCH_FROM(num,obj)\
if constexpr (get_field_count<T>::value ==0){ \
		return std::make_tuple(); \
}\

#define ELSE_IF_BRANCH_FROM(num,obj)\
else if constexpr (get_field_count<T>::value ==num){ \
		auto& [VAR_LIST(num)] = obj; \
		return std::tie(VAR_LIST(num)); \
}\

#define TOTUPLES_FROM(a,n) LOOP(n,IF_BRANCH_FROM,ELSE_IF_BRANCH_FROM,a)

		template<typename T>
		constexpr auto as_tuple(T& a)
		{
			EVAL(TOTUPLES_FROM(a, 32));
		}




		template<typename tup>
		struct extract_tuple_pointer;

		template<typename ...P>
		struct extract_tuple_pointer<std::tuple<P*...>>
		{
			using types = type_list<P...>;
		};

		template<typename T>
		struct extract_member_type_list
		{
			using tuple_t = decltype(as_tuple<T>());
			using types = extract_tuple_pointer<tuple_t>::types;
		};

	}


	template<typename T>
	using extract_member_type_list_t = details::extract_member_type_list<T>::types;

}
