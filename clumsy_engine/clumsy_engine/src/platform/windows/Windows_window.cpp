


#include "Windows_window.h"
#include "log.h"
#include "helper_utils.h"
#include "GLFW/glfw3.h"

namespace clumsy_engine
{

	static bool s_GLFW_initialized = false;


	Windows_Window::Windows_Window(const Window_Property& p)
	{
		init(p);
	}

	Windows_Window::~Windows_Window()
	{

	}

	void Windows_Window::on_update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void Windows_Window::init(const Window_Property& p)
	{
		m_window_data.width = p.width;
		m_window_data.height = p.height;
		m_window_data.title = p.title;

		CE_CORE_TRACE("creating window {0},({1},{2})", p.title, p.width, p.height);

		if (!s_GLFW_initialized)
		{
			int success = glfwInit();
			CE_CORE_ASSERT(success, "glfw init failed!");
			s_GLFW_initialized = true;
		}

		m_window = glfwCreateWindow(m_window_data.width, m_window_data.height, m_window_data.title.c_str(), nullptr, nullptr);
		if (m_window)
		{
			CE_CORE_TRACE("windows {0} is created!", (void*)m_window);
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_window_data);
		set_vertical_sync(true);

	}

	void Windows_Window::shut_down()
	{
		glfwDestroyWindow(m_window);
	}

	unsigned int Windows_Window::get_width() const
	{
		return m_window_data.width;

	}
	unsigned int Windows_Window::get_height() const
	{
		return m_window_data.height;
	}

	void Windows_Window::set_event_callback(const Event_Callback& event_call_back)
	{
		m_window_data.event_callback = event_call_back;
	}
	void Windows_Window::set_vertical_sync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_window_data.v_sync = enabled;
	}

	bool Windows_Window::is_verticla_sync()const
	{
		return m_window_data.v_sync;
	}


	Window& Window::create(const Window_Property& p)
	{
		static std::unique_ptr<Windows_Window> w = std::make_unique<Windows_Window>(p);
		return *w;
	}

}