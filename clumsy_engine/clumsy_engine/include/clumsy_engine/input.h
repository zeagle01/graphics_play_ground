
#pragma once


#include "clumsy_engine_export.h"
#include <array>

namespace clumsy_engine
{

	struct mouse_pos
	{
		float x;
		float y;
	};

	class  CLUMSY_ENGINE_EXPORT  Input
	{
	public:
		static inline bool is_key_pressed(int keycode)
		{
			return Input::get_singleton().is_key_pressed_impl(keycode);
		};
		static inline bool is_mouse_button_pressed(int button)
		{
			return Input::get_singleton().is_mouse_button_pressed_impl(button);
		};

		static inline mouse_pos get_mouse_position()
		{
			return Input::get_singleton().get_mouse_position_impl();
		}

		static Input& get_singleton();

	protected:
		virtual bool is_key_pressed_impl(int keycode) = 0;
		virtual bool is_mouse_button_pressed_impl(int button) = 0;
		virtual mouse_pos get_mouse_position_impl() = 0;
	};

	struct Drag_Delta_Computer
	{
		std::array<float, 2> operator()(int button)
		{
			bool pressing = Input::is_mouse_button_pressed(button);
			auto mouse_pos = Input::get_mouse_position();

			std::array<float, 2> ret;
			ret[0] = 0.f;
			ret[1] = 0.f;

			if (pressing)
			{
				float delta_x = mouse_pos.x - m_mouse_pos_x;
				float delta_y = mouse_pos.y - m_mouse_pos_y;

				ret[0] = delta_x;
				ret[1] = delta_y;
			}

			m_mouse_pos_x = mouse_pos.x;
			m_mouse_pos_y = mouse_pos.y;

			return ret;
		}

		bool m_release = false;
		float m_mouse_pos_x = 0.f;
		float m_mouse_pos_y = 0.f;

	};

}