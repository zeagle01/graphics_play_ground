
#pragma once

#include "type_list.h"
#include "static_loop.h"
#include <type_traits> 


namespace clumsy_lib
{
	//////////////////For_Each_Type_Enumerate
	template<typename tl>
	using For_Each_Type_Enumerate = For_Each_Type_Enumerate_With_Index_Sequence<tl, std::make_index_sequence<get_size_v<tl>>>;


	//////////////////double_for_each_type
	template<typename tl0, typename tl1, typename F, typename ctl0 = tl0, typename ctl1 = tl1, typename ...P>
	static void double_for_each_type(P&&... p)
	{
		if constexpr (!is_empty<ctl0> && !is_empty<ctl1>)
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
}




