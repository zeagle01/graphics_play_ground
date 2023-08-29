module;

#include "clumsy_lib/field_defines.h"

#include <string>
#include <array>

export module quick_shell: ui_components;


namespace quick_shell
{
	using arr2 = std::array<float, 2>;
	using arr3 = std::array<float, 3>;

	using arr2i = std::array<int, 2>;

	struct no_extra {};


	export  template<typename T>
	struct range
	{
		T min;
		T max;
	};

	using range_f = range<float>;
	using range_i = range<int>;

	export struct ui_component
	{
		CE_ADD_NODE(check_box,		CE_TYPE(bool)			CE_FIELD(extra_data, no_extra));
		CE_ADD_NODE(slider_bar,		CE_TYPE(float)			CE_FIELD(extra_data, range_f));
		CE_ADD_NODE(slider_bar2,	CE_TYPE(arr2)			CE_FIELD(extra_data, range_f));
		CE_ADD_NODE(slider_bar3,	CE_TYPE(arr3)			CE_FIELD(extra_data, range_f));

		CE_ADD_NODE(slider_bar2i,	CE_TYPE(arr2i)			CE_FIELD(extra_data, range_i));

		CE_ADD_NODE(text_line,		CE_TYPE(std::string)	CE_FIELD(extra_data, no_extra));
	};
}
