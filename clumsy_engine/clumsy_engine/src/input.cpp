

#include "input.h"
#include <memory>
#include "windows_input.h"

namespace clumsy_engine
{

//	bool Input::is_key_pressed(int keycode)
//	{
//		return Input::get_singleton().is_key_pressed_impl(keycode);
//	};
//	bool Input::is_mouse_button_pressed(int button) 
//	{
//		return Input::get_singleton().is_mouse_button_pressed_impl(button);
//	};
//
//	mouse_pos Input::get_mouse_position()
//	{
//		return Input::get_singleton().get_mouse_position_impl();
//	}


	Input& Input::get_singleton()
	{
		static std::unique_ptr<Input> singleton = std::make_unique<Windows_Input>();
		return *singleton;
	}

}