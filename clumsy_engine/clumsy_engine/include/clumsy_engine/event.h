

#pragma once

#include <string>
#include <functional>

namespace clumsy_engine
{
	enum class Event_Type
	{
		None,
		Window_Close, Window_Resize, WindowFocus, Window_Lost_Focus, Window_Moved,
		App_Tick, App_Update, App_Render,
		Key_Pressed, Key_Released,Key_Typed,
		Mouse_Button_Pressed, Mouse_Button_Rleased, Mouse_Moved, Mouse_Scrolled,
		Event_Type_Size
	};

	enum Event_Category
	{
		None = 0,
		Application_Cate = 1 << 0,
		Input = 1 << 1,
		Keyboard = 1 << 2,
		Mouse = 1 << 3,
		MouseButton = 1 << 4
	};


#define EVENT_CLASS_TYPE(type) static Event_Type get_static_type() {return Event_Type::##type; }\
								virtual Event_Type get_dynamic_type() const override{return get_static_type();}\
								virtual std::string get_name() const override{return #type;}\

#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flag()const override { return category; };


	class Event
	{
	public:
		virtual  Event_Type get_dynamic_type()const = 0;
		virtual std::string get_name()const = 0;

		virtual int get_category_flag()const = 0;


		virtual std::string to_string() const { return get_name(); }

		inline bool is_in_category(Event_Category category)
		{
			return get_category_flag() & category;
		}

		bool is_handled() { return m_is_handled; }

	private:
		bool m_is_handled = false;
	};

}