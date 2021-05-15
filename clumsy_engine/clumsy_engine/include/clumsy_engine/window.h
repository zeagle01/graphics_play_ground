

#pragma once

#include <functional>
#include "event.h"
#include <string>
#include "log.h"
#include <memory>



namespace clumsy_engine
{

	struct Window_Property
	{
		std::string title;
		unsigned int width;
		unsigned int height;
	};


	class Window
	{
	public:
		using  Event_Callback = std::function<bool(Event&)>;

		virtual ~Window() 
		{
			CE_CORE_INFO("destroy window {0}", (void*)this);
		};

		virtual void on_update() = 0;
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;

		virtual void set_event_callback(const Event_Callback& event_call_back) = 0;
		virtual void set_vertical_sync(bool enabled) = 0;
		virtual bool is_verticla_sync()const = 0;

		virtual void* get_window_native() const = 0;


		static std::unique_ptr<Window> create(const Window_Property& p = { .title = "clumsy_engine",.width = 800,.height = 600 });
	};

}