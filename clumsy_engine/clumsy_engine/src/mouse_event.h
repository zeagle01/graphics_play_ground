
#pragma once

#include "event.h"
#include <sstream>

namespace clumsy_engine
{
    class Mouse_Moved_Event:public Event
    {
        public:
        Mouse_Moved_Event(int x,int y):m_x(x),m_y(y){}

        EVENT_CLASS_TYPE(Mouse_Moved)
        EVENT_CLASS_CATEGORY(Event_Category::Mouse|Event_Category::Input)

		virtual std::string to_string() const override
        {
            std::stringstream ss;
            ss<<"Mouse_Moved_Event: "<<m_x<<" "<<m_y;
            return ss.str();
             }

        private:
        int m_x;
        int m_y;

    };

    class Mouse_Scrolled_Event:public Event
    {
        public:
        Mouse_Scrolled_Event(int x,int y):m_x_offset(x),m_y_offset(y){}

        EVENT_CLASS_TYPE(Mouse_Scrolled)
        EVENT_CLASS_CATEGORY(Event_Category::Mouse|Event_Category::Input)

		virtual std::string to_string() const override
        {
            std::stringstream ss;
            ss<<"Mouse_Scrolled_Event: "<<m_x_offset<<" "<<m_x_offset;
            return ss.str();
             }

        private:
        int m_x_offset;
        int m_y_offset;

    };


    class Mouse_Button_Event:public Event
    {
        public:
Mouse_Button_Event(int button):m_button(button){}
EVENT_CLASS_CATEGORY(Event_Category::Input|Event_Category::MouseButton)


        protected:
        int m_button;


    };

    class Mouse_Button_Pressed_Event:public Mouse_Button_Event
    {
        public:
Mouse_Button_Pressed_Event(int button):Mouse_Button_Event(button){}

EVENT_CLASS_TYPE(Mouse_Button_Pressed)

		std::string to_string() const override
        {

			std::stringstream ss;
			ss << "Mouse_Button_Pressed_Event: " << m_button;
			return ss.str();
        }

    };

    class Mouse_Button_Released_Event:public Mouse_Button_Event
    {
        public:
Mouse_Button_Released_Event(int button):Mouse_Button_Event(button){}

EVENT_CLASS_TYPE(Mouse_Button_Rleased)

		std::string to_string() const override
        {

			std::stringstream ss;
			ss << "Mouse_Button_Released_Event: " << m_button;
			return ss.str();
        }

    };


}
