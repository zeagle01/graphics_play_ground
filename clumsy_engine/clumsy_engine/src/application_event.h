

#pragma once

#include "event.h"
#include <sstream>
#include <string>


namespace clumsy_engine
{
	class Window_Resize_Event :public Event
	{
	public:
		Window_Resize_Event(int w, int h) :m_width(w), m_height(h) {}
		EVENT_CLASS_TYPE(Window_Resize)
			EVENT_CLASS_CATEGORY(Event_Category::Application_Cate)


			std::string to_string() const override
		{
			std::stringstream ss;
			ss << "Window_Resize_Event: " << m_width << " " << m_height;
			return ss.str();
		}

	private:
		int m_width;
		int m_height;
	};


    class Window_Close_Event: public Event
    {
        public:
EVENT_CLASS_TYPE(Window_Close)
EVENT_CLASS_CATEGORY(Event_Category::Application_Cate)

    };

    class Window_Tick_Event: public Event
    {
        public:
EVENT_CLASS_TYPE(App_Tick)
EVENT_CLASS_CATEGORY(Event_Category::Application_Cate)

    };

    class Window_Render_Event: public Event
    {
        public:
EVENT_CLASS_TYPE(App_Render)
EVENT_CLASS_CATEGORY(Event_Category::Application_Cate)

    };

}