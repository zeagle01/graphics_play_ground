


#pragma once

#include "window.h"
#include "ref.h"

#include <any>


struct GLFWwindow;

namespace clumsy_engine
{

	class Graphic_Context;

	class Windows_Window :public Window
	{
	public:
		Windows_Window(const Window_Property& p);
		virtual ~Windows_Window() ;

		void on_update() override;
		unsigned int get_width() const override;
		unsigned int get_height() const override;

		void set_event_callback(const Event_Callback& event_call_back) override;
		void set_vertical_sync(bool enabled) override;
		bool is_verticla_sync()const override;

		virtual void* get_window_native() const override;

	private:
		virtual void init(const Window_Property& p);
		virtual void shut_down();

	private:

		GLFWwindow* m_window;

		Ref<Graphic_Context> m_context;


		struct Window_Data
		{
			std::string title;
			unsigned int width, height;
			bool v_sync;
			Event_Callback event_callback;

		};
		Window_Data m_window_data;
	};
}