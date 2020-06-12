


#pragma once

#include <sstream>

#include "event.h"

namespace clumsy_engine
{
	class Key_Event : public Event
	{

	public:
		EVENT_CLASS_CATEGORY(Event_Category::Keyboard|Event_Category::Input)
	protected:
		Key_Event(int key_code) :m_key_code(key_code) {}
		int m_key_code;
	};

	class Key_Pressed_Event:public Key_Event
	{
	public:
		Key_Pressed_Event(int key_code, int repeat_count) :Key_Event(key_code), m_repeat_count(repeat_count) {}

		EVENT_CLASS_TYPE(Key_Pressed)


		std::string to_string() const override
		{ 
			std::stringstream ss;
			ss << "Key_Pressed_Event: " << m_key_code << " (" << m_repeat_count << ") " << "repeat)";
			return ss.str();
		}
	private:
		int m_repeat_count;
	};

	class Key_Release_Event:public Key_Event
	{
	public:
		Key_Release_Event(int key_code) :Key_Event(key_code)  {}

		EVENT_CLASS_TYPE(Key_Released)


		std::string to_string() const override
		{ 
			std::stringstream ss;
			ss << "Key_Release_Event: " << m_key_code;
			return ss.str();
		}
	};

	inline std::ostream& operator<<(std::ostream& os, Event& e)
	{
		return	os << e.to_string();
	}

}