


#pragma once

#include "layer.h"






namespace clumsy_engine
{


	class Layer;
	class Event;

	class Mouse_Button_Pressed_Event;
	class Mouse_Button_Released_Event;
	class Mouse_Moved_Event;
	class Mouse_Scrolled_Event;
	class Key_Pressed_Event;
	class Key_Release_Event;
	class Window_Resize_Event;
	class Key_Typed_Event;

	class Imgui_Layer :public Layer
	{
	public:
		Imgui_Layer();
		~Imgui_Layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void on_update() override;
		virtual void on_event(Event& e) override;

	private:
		bool on_mouse_button_pressed_event(Mouse_Button_Pressed_Event& e);
		bool on_mouse_button_release_event(Mouse_Button_Released_Event& e);
		bool on_mouse_moved_event(Mouse_Moved_Event& e);
		bool on_mouse_scrolled_event(Mouse_Scrolled_Event& e);
		bool on_key_pressed_event(Key_Pressed_Event& e);
		bool on_key_released_event(Key_Release_Event& e);
		bool on_key_typed_event(Key_Typed_Event& e);
		bool window_resize_event(Window_Resize_Event& e);

	};

}