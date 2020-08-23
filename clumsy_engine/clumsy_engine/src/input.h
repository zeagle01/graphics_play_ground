
#pragma once


namespace clumsy_engine
{

	struct mouse_pos
	{
		float x;
		float y;
	};

	class Input
	{
	public:
		static inline bool is_key_pressed(int keycode) { get_singleton().is_key_pressed_impl(keycode); };
		static inline bool is_mouse_button_pressed(int button) { get_singleton().is_mouse_button_pressed_impl(button); };
		static inline mouse_pos get_mouse_position() { return get_singleton().get_mouse_position_impl(); }

		static Input& get_singleton();

	protected:
		virtual bool is_key_pressed_impl(int keycode) = 0;
		virtual bool is_mouse_button_pressed_impl(int button) = 0;
		virtual mouse_pos get_mouse_position_impl() = 0;
	};

}