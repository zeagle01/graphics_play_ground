
#pragma once
#include <type_traits>
#include <utility>

namespace clumsy_lib
{
	/////////////////////For_Each_Type///////////////////////
	template<typename Type_List>
	struct For_Each_Type;

	//public:
	template< template <typename ...> typename Type_Holder, typename ...TP>
	struct For_Each_Type<Type_Holder<TP...>>
	{
		template< typename F, typename ...P>
		static void apply(P&&...p)
		{
			(typename F::template apply<TP>(std::forward<P>(p)...), ...);
		}
	};


	/////////////////////For_Each_In_Range///////////////////////
	template<typename Seq >
	struct For_Each_In_Range_Imp;

	template<size_t ... N>
	struct For_Each_In_Range_Imp<std::index_sequence< N...>>
	{
		template<typename F, typename ...P>
		static void apply(P&&...p)
		{
			((typename F:: template apply<N>(std::forward<P>(p)...)), ...);
		}
	};

	//public:
	template<size_t N>
	using  For_Each_In_Range = For_Each_In_Range_Imp<std::make_index_sequence<N>>;



	/////////////////////For_Each_Type_Enumerate///////////////////////
	template<typename tl, typename sq>
	struct For_Each_Type_Enumerate_With_Index_Sequence;

	//public:
	template<typename ...Ts, template <typename ... > typename Type_Holder, size_t... Is>
	struct For_Each_Type_Enumerate_With_Index_Sequence< Type_Holder<Ts...>, std ::index_sequence<Is... >>
	{
		template<typename F, typename...P>
		static void apply(P&&...p) 
		{ 
			(F::template apply<Is, Ts>(std::forward<P>(p)...),...); //Is number and Ts nubmer must match!
		}
	};

}

