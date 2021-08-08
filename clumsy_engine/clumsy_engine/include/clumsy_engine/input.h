
#pragma once


#include "clumsy_engine_export.h"
#include <array>
#include <map>
#include "log.h"
#include "mouse_button_codes.h"

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


	class Drag_Delta_Computer
	{
	public:
		Drag_Delta_Computer()
		{
			m_delta_pos[CE_MOUSE_BUTTON_LEFT] = { 0,0 };
			m_delta_pos[CE_MOUSE_BUTTON_RIGHT] = { 0,0 };
			m_delta_pos[CE_MOUSE_BUTTON_MIDDLE] = { 0,0 };
		}

		void compute()
		{
			auto mouse_p = Input::get_mouse_position();
			for (auto& it : m_delta_pos)
			{
				it.second = compute_button_delta(mouse_p.x, mouse_p.y, it.first);
			}
			m_mouse_pos_x = mouse_p.x;
			m_mouse_pos_y = mouse_p.y;
		}

		std::array<float, 2> get(int button)
		{
			return m_delta_pos[button];
		}

	private:

		std::array<float, 2> compute_button_delta(float mouse_x, float mouse_y, int button)
		{
			bool pressing = Input::is_mouse_button_pressed(button);
			std::array<float, 2> ret{ 0,0 };
			if (pressing)
			{
				ret[0] = mouse_x - m_mouse_pos_x;
				ret[1] = -(mouse_y - m_mouse_pos_y);
			}
			return ret;
		}

		float m_mouse_pos_x = 0.f;
		float m_mouse_pos_y = 0.f;
		std::map<int, std::array<float, 2>> m_delta_pos;
	};


}