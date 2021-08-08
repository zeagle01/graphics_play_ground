

#include "windows_input.h"

#include "application.h"
#include "window.h"

namespace clumsy_engine

{

	bool Windows_Input::is_key_pressed_impl(int keycode)
	{
		const auto& window = static_cast<GLFWwindow*>(Application::get_singleton().get_window().get_window_native());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Windows_Input::is_mouse_button_pressed_impl(int button)
	{
		const auto& window = static_cast<GLFWwindow*>(Application::get_singleton().get_window().get_window_native());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS ;

		}

	Mouse_Pos Windows_Input::get_mouse_position_impl()
	{
		const auto& window = static_cast<GLFWwindow*>(Application::get_singleton().get_window().get_window_native());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { float(xpos),float(ypos) };
	}
}