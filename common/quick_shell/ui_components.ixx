module;

#include "clumsy_lib/field_defines.h"

export module main_window: ui_components;


namespace quick_shell
{
	export struct ui_component
	{
		CE_ADD_NODE(check_box, CE_TYPE(bool));
		CE_ADD_NODE(slider_bar, CE_TYPE(float));
	};
}
