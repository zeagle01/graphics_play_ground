
#pragma once
#include <type_traits>

namespace clumsy_lib
{
	template<typename Type_List>
	struct For_Each_Type;

	template< template <typename ...> typename Type_Holder, typename ...TP>
	struct For_Each_Type<Type_Holder<TP...>>
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
		template<typename F, typename ...P>
		static void apply(P&&...p)
		{
			((typename F:: template apply<N>(std::forward<P>(p)...)), ...);
		}
	};

	template<size_t N>
	using  For_Each_In_Range = For_Each_In_Range_Imp<std::make_index_sequence<N>>;
}

