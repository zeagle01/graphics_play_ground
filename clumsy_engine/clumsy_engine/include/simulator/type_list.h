
#pragma once

#include <concepts>



namespace clumsy_engine
{

	template<typename... P>
	struct type_list {};


	//front
	template<typename L >
	struct front_imp;

	template<typename H, typename... T>
	struct front_imp<type_list<H, T...>>
	{
		using type = H;
	};

	template<typename L>
	using front = typename front_imp<L>::type;

	//pop front
	template<typename L>
	struct pop_front_imp;

	template<typename H, typename... T>
	struct pop_front_imp<type_list<H, T...>>
	{
		using type = type_list<T...>;
	};

	template<typename L>
	using pop_front = typename pop_front_imp<L>::type;

	//push front
	template<typename H, typename L >
	struct push_front_imp;

	template<typename H, typename... T>
	struct push_front_imp<H, type_list< T...>>
	{
		using type = type_list<H, T...>;
	};

	template<typename H, typename L>
	using push_front = typename push_front_imp<H, L>::type;

	//get_nth_element
	template<typename L, size_t n>
	struct get_nth_element_imp
	{
		using type = typename get_nth_element_imp<pop_front<L>, n - 1>::type;
	};

	template<typename L>
	struct get_nth_element_imp<L, 0>
	{
		using type = front<L>;
	};

	template<typename L, size_t n>
	using get_nth_element = typename get_nth_element_imp<L, n>::type;

	//is_empty
	template<typename L>
	struct is_empty_imp
	{
		static inline constexpr  bool value = false;
	};

	template<>
	struct is_empty_imp<type_list<>>
	{
		static inline constexpr  bool value = true;
	};

	template<typename L>
	inline constexpr bool is_empty = is_empty_imp<L>::value;


	//is_in
	template<typename T,typename L>
	struct is_in_imp
	{
		static inline constexpr bool value = false;
	};

	template<typename T>
	struct is_in_imp<T,type_list<>>
	{
		static inline constexpr bool value = false;
	};

	template<typename T, typename H, typename ...P>
	struct is_in_imp<T, type_list<H, P...>>
	{
		static inline constexpr bool value = std::conditional_t<
			std::is_same_v<T, H>,
			std::true_type,
			is_in_imp<T, type_list<P...>>
			>::value;
	};

	template<typename T,typename L>
	inline constexpr bool is_in= is_in_imp<T, L>::value;


	template<typename T,typename tl>
	concept Type_In_List = is_in<T, tl>;


	///
	template<typename tl, typename F, typename ...P>
	static void for_each_type(P&&... p)
	{
		if constexpr (!is_empty<tl>)
		{
			using current_t = front<tl>;

			F::template apply<current_t>(std::forward<P>(p)...);

			using poped_list = pop_front<tl>;

			for_each_type<poped_list, F>(std::forward<P>(p)...);
		}
		else
		{
			return;
		}

	};


	template<typename tl0, typename tl1, typename F, typename ctl0 = tl0, typename ctl1 = tl1, typename ...P>
	static void double_for_each_type(P&&... p)
	{
		if constexpr (!is_empty<ctl0>&&!is_empty<ctl1>)
		{
			using current_t0 = front<ctl0>;

			using current_t1 = front<ctl1>;

			F::template apply<current_t0, current_t1>(std::forward<P>(p)...);

			using poped_list0 = pop_front<ctl0>;
			using poped_list1 = pop_front<ctl1>;

			double_for_each_type<tl0, tl1, F, poped_list0, poped_list1 >(std::forward<P>(p)...);
		}
		else if constexpr (is_empty<tl0> && !is_empty<tl1>)
		{
			using current_t0 = front<tl0>;

			using current_t1 = front<ctl1>;

			F::template apply<current_t0, current_t1>(std::forward<P>(p)...);

			using poped_list0 = pop_front<tl0>;
			using poped_list1 = pop_front<ctl1>;

			double_for_each_type<tl0, tl1, F, poped_list0, poped_list1>(std::forward<P>(p)...);
		}
		else 
		{
			return;
		}

	};

}