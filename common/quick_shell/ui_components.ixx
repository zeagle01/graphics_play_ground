module;

#include "clumsy_lib/field_defines.h"

#include <string>
#include <array>

export module main_window: ui_components;


namespace quick_shell
{
	using arr2 = std::array<float, 2>;
	using arr3 = std::array<float, 3>;

	struct no_extra {};


	export struct range
	{
		float min;
		float max;
	};

	export struct ui_component
	{
		CE_ADD_NODE(check_box,		CE_TYPE(bool)			CE_FIELD(extra_data, no_extra));
		CE_ADD_NODE(slider_bar,		CE_TYPE(float)			CE_FIELD(extra_data, range));
		CE_ADD_NODE(slider_bar2,	CE_TYPE(arr2)			CE_FIELD(extra_data, range));
		CE_ADD_NODE(slider_bar3,	CE_TYPE(arr3)			CE_FIELD(extra_data, range));
		CE_ADD_NODE(text_line,		CE_TYPE(std::string)	CE_FIELD(extra_data, no_extra));
	};
}
