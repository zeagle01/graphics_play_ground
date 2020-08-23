

#pragma once

#include "input.h"

#include "GLFW/glfw3.h"


namespace clumsy_engine
{


	class Windows_Input :public Input
	{
	protected:
		virtual bool is_key_pressed_impl(int keycode) override;
		virtual bool is_mouse_button_pressed_impl(int button) override;
		virtual mouse_pos get_mouse_position_impl() override;

	};

}