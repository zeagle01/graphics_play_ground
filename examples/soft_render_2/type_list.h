#pragma once

#include <concepts>

namespace soft_render
{

	template<typename ...P>
	struct type_list;


	//front
	namespace details
	{
		template<typename tl>
		struct front_imp;

		template<typename H, typename ...P>
		struct front_imp < type_list<H, P...>>
		{
			using type = H;
		};
	}
	template<typename tl> using front_t = typename details::front_imp<tl>::type;


	//pop front
	namespace details
	{
		template<typename tl>
		struct pop_front_imp;

		template<typename H, typename ...P>
		struct pop_front_imp<type_list<H, P...>>
		{
			using type = type_list<P...>;
		};
	}
	template<typename T> using pop_front_t = typename details::pop_front_imp<T>::type;


	//push front
	namespace details
	{
		template<typename tl, typename H>
		struct push_front_imp;

		template<typename H, typename ...P>
		struct push_front_imp< type_list<P...>, H>
		{
			using type = type_list<H, P...>;
		};
	}
	template<typename tl, typename H> using push_front_t = typename details::push_front_imp<tl, H>::type;


	//get nth element
	namespace details
	{
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
	}
	template<typename tl, int N> using get_nth_element_t = typename details::get_nth_element_imp<tl, N>::type;

	//get size
	namespace details
	{
		template<typename tl>
		struct	get_size_imp;

		template<typename...Ts>
		struct	get_size_imp<type_list<Ts...>>
		{
			static constexpr int value = sizeof...(Ts);
		};
	}
	template<typename tl> constexpr int get_size_v = get_size_imp<tl>::value;

	//get index 
	namespace details
	{
		template<typename tl, typename T, int N>
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
					return N - sizeof...(Ts) - 1;
				}
				else
				{
					return get_index_imp<type_list<Ts...>, T, N>::get_value();
				}
			}
		};
	}
	template<typename tl, typename T> constexpr int get_index_v = details::get_index_imp<tl, T, get_size_v<tl>>::get_value();

	//is empty
	namespace details
	{
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
	}
	template<typename tl> constexpr bool is_empty_v = is_empty_imp<tl>::value;

	//is_in
	namespace details
	{
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
	}
	template<typename T, typename tl> inline constexpr bool is_in = is_in_imp<T, tl>::value;
	template<typename T, typename tl> concept type_in_list = is_in<T, tl>;


	//////////////////////
	using empty_type_list = type_list<>;



	/////////////////////For_Each_Type///////////////////////
	namespace details
	{
		template<typename tl>
		struct for_each_type;

		//public:
		template< template <typename ...> typename tl, typename ...TP>
		struct for_each_type<tl<TP...>>
		{
			// template<typename T> struct F{ apply (p) {} }
			template< typename F, typename ...P>
			static void apply(P&&...p)
			{
				(typename F::template apply<TP>(std::forward<P>(p)...), ...);
			}
		};

	}
	template<typename tl>
	using for_each_type = details::for_each_type<tl>;


}
