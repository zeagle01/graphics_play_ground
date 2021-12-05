
#pragma once

#include <concepts>
#include <tuple>


namespace clumsy_lib 
{

	template<typename ...P>
	struct type_list;


	//front
	template<typename tl>
	struct front_imp;

	template<typename H, typename ...P>
	struct front_imp < type_list<H, P...>>
	{
		using type = H;
	};
	template<typename tl>
	using front_t = typename front_imp<tl>::type;


	//pop front
	template<typename tl>
	struct pop_front_imp;

	template<typename H, typename ...P>
	struct pop_front_imp<type_list<H, P...>>
	{
		using type = type_list<P...>;
	};

	template<typename T>
	using pop_front_t = typename pop_front_imp<T>::type;


	//push front
	template<typename tl, typename H>
	struct push_front_imp;

	template<typename H, typename ...P>
	struct push_front_imp< type_list<P...>, H>
	{
		using type = type_list<H, P...>;
	};

	template<typename tl, typename H>
	using push_front_t = typename push_front_imp<tl, H>::type;


	//get nth element
	template<typename tl, int N>
	struct get_nth_element_imp
	{
		using poped_list = typename pop_front_t<tl>;
		using type = typename get_nth_element_imp<poped_list, N - 1>::type;
	};

	template<typename tl >
	struct get_nth_element_imp<tl, 0>
	{
		using type = typename front_t<tl>;
	};

	template<typename tl, int N>
	using get_nth_element_t = typename get_nth_element_imp<tl, N>::type;

	//get size
	template<typename tl>
	struct	get_size_imp;

	template<typename...Ts>
	struct	get_size_imp<type_list<Ts...>>
	{
		static constexpr int value = sizeof...(Ts);
	};

	template<typename tl>
	constexpr int get_size_v = get_size_imp<tl>::value;

	//get index 
	template<typename tl,typename T,int N>
	struct get_index_imp
	{
		static constexpr int get_value() { return -1; }
	};

	template<typename T, int N, typename H, typename... Ts>
	struct get_index_imp<type_list<H, Ts...>, T, N>
	{

		static constexpr int get_value() 
		{
			if constexpr (std::is_same_v<T, H>)
			{
				return N - sizeof...(Ts)-1;
			}
			else
			{
				return get_index_imp<type_list<Ts...>, T, N>::get_value();
			}
		}
	};

	template<typename tl, typename T>
	constexpr int get_index_v = get_index_imp<tl, T, get_size_v<tl>>::get_value();

	//is empty
	template<typename tl>
	struct is_empty_imp
	{
		static constexpr bool value = false;
	};

	template<>
	struct is_empty_imp<type_list<> >
	{
		static constexpr bool value = true;
	};

	template<typename tl>
	constexpr bool is_empty_v = is_empty_imp<tl>::value;

	//is_in
	template<typename T, typename L>
	struct is_in_imp
	{
		static inline constexpr bool value = false;
	};

	template<typename T>
	struct is_in_imp<T, type_list<>>
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

	template<typename T, typename L>
	inline constexpr bool is_in = is_in_imp<T, L>::value;


	template<typename T, typename tl>
	concept Type_In_List = is_in<T, tl>;


	//////////////////////
	using empty_type_list = type_list<>;




	///
	template<  typename F, typename tl >
	struct for_each_type_inner;

	template<  typename F, typename ...T>
	struct for_each_type_inner< F, type_list<T...>>
	{
		static void apply(F&& f)
		{
			(f.operator() < T > (), ...);
		}

	};

	template<typename tl, typename F, typename ...P>
	static void for_each_type(P&&... p)
	{
		auto parameterized_func = [&]<typename T>()
		{
			F::template apply<T>(std::forward<P>(p)...);
		};
		using func_t = decltype(parameterized_func);

		for_each_type_inner<func_t, tl >::apply(std::forward<func_t>(parameterized_func));
	}


	template<typename tl, typename F, int N>
	struct For_Each_Type_With_Index_Imp;

	template< typename F ,int N>
	struct For_Each_Type_With_Index_Imp<type_list<>, F, N>
	{
		template<typename...P>
		static void apply(P&&...p) {  }
	};

	template<typename F, int N, typename H, typename ...Ts  >
	struct For_Each_Type_With_Index_Imp<type_list<H, Ts...>, F, N>
	{
		template<typename...P>
		static void apply(P&&...p) 
		{ 
			F::template apply<H>(N - sizeof...(Ts) - 1, std::forward<P>(p)...);
			For_Each_Type_With_Index_Imp<type_list<Ts...>, F, N>::apply(std::forward<P>(p)...);
		}
	};

	template<typename tl, typename F >
	using For_Each_Type_With_Index = For_Each_Type_With_Index_Imp<tl, F, get_size_v<tl>>;


	template<typename tl0, typename tl1, typename F, typename ctl0 = tl0, typename ctl1 = tl1, typename ...P>
	static void double_for_each_type(P&&... p)
	{
		if constexpr (!is_empty<ctl0>&&!is_empty<ctl1>)
		{
			using current_t0 = front_t<ctl0>;

			using current_t1 = front_t<ctl1>;

			F::template apply<current_t0, current_t1>(std::forward<P>(p)...);

			using poped_list0 = pop_front_t<ctl0>;
			using poped_list1 = pop_front_t<ctl1>;

			double_for_each_type<tl0, tl1, F, poped_list0, poped_list1 >(std::forward<P>(p)...);
		}
		else if constexpr (is_empty<tl0> && !is_empty<tl1>)
		{
			using current_t0 = front_t<tl0>;

			using current_t1 = front_t<ctl1>;

			F::template apply<current_t0, current_t1>(std::forward<P>(p)...);

			using poped_list0 = pop_front_t<tl0>;
			using poped_list1 = pop_front_t<ctl1>;

			double_for_each_type<tl0, tl1, F, poped_list0, poped_list1>(std::forward<P>(p)...);
		}
		else 
		{
			return;
		}

	};


	//better force each

	template<typename Type_List>
	struct For_Each_Type;

	template< typename ...TP>
	struct For_Each_Type<type_list<TP...>>
	{
		template< typename F, typename ...P>
		static void apply(P&&...p)
		{
			(typename F::template apply<TP>(std::forward<P>(p)...), ...);
		}
	};


	template<typename Seq >
	struct For_Each_In_Range_Imp;

	template<size_t ... N>
	struct For_Each_In_Range_Imp<std::index_sequence< N...>>
	{
		template<typename F,typename ...P>
		static void apply(P&&...p)
		{
			((typename F:: template apply<N>(std::forward<P>(p)...)), ...);
		}
	};

	template<size_t N>
	using  For_Each_In_Range = For_Each_In_Range_Imp<std::make_index_sequence<N>>;

}

