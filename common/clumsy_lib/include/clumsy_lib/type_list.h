
#pragma once

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

template<typename H,typename ...P>
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

template<typename tl,typename H>
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

template<typename tl,int N>
using get_nth_element_t = typename get_nth_element_imp<tl, N>::type;

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


