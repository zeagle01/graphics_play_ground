


#include "Windows_window.h"
#include "log.h"
#include "helper_utils.h"

#include "mouse_event.h"
#include "key_event.h"
#include "application_event.h"

#include "glad/glad.h" //put glad first before other opengl include
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
		CE_CORE_TRACE("delete Windows_Window {0}", (void*)this);
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

			glfwSetErrorCallback([](int code, const char* message)
			{
				CE_ERROR("glfw error ({0}):{1}", code, message);
			});

			s_GLFW_initialized = true;
		}

		m_window = glfwCreateWindow(m_window_data.width, m_window_data.height, m_window_data.title.c_str(), nullptr, nullptr);
		if (m_window)
		{
			CE_CORE_TRACE("windows {0} is created!", (void*)m_window);
		}


		glfwMakeContextCurrent(m_window);

		//right after make context current(need a context)
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CE_CORE_ASSERT(status, "glad load failed");

		glfwSetWindowUserPointer(m_window, &m_window_data);
		set_vertical_sync(true);

		//set GLFW callbacks
		glfwSetWindowSizeCallback(m_window,
			[](GLFWwindow* window, int w, int h) {

			Window_Data& window_data = *(Window_Data*)glfwGetWindowUserPointer(window);

			window_data.width = w;
			window_data.height = h;

			Window_Resize_Event e(w, h);
			window_data.event_callback(e);

			//CE_TRACE("resize {0} {1}", w, h);
		}
		);

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{

			Window_Data& window_data = *(Window_Data*)glfwGetWindowUserPointer(window);

			Window_Close_Event e;

			window_data.event_callback(e);
		}
		);

		glfwSetKeyCallback(m_window, [](GLFWwindow* window,int key,int scan_code,int action,int mods)
		{
			Window_Data& window_data = *(Window_Data*)glfwGetWindowUserPointer(window);

			switch (action)
			{

				case GLFW_PRESS:
				{
					Key_Pressed_Event e(key,0);
					window_data.event_callback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					Key_Pressed_Event e(key,1);
					window_data.event_callback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					Key_Release_Event e(key);
					window_data.event_callback(e);
					break;
				}

			}
		}
		);

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int unicode) {
			Window_Data& window_data = *(Window_Data*)glfwGetWindowUserPointer(window);
			Key_Typed_Event e(unicode);
			window_data.event_callback(e);
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window,int button,int action,int mods)
		{
			Window_Data& window_data = *(Window_Data*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					Mouse_Button_Pressed_Event e(button);
					window_data.event_callback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					Mouse_Button_Released_Event e(button);
					window_data.event_callback(e);
					break;
				}
			}

		}
		);

		glfwSetScrollCallback(m_window, [](GLFWwindow*window,double xoffset,double yoffset)
		{

			Window_Data& window_data = *(Window_Data*)glfwGetWindowUserPointer(window);

			Mouse_Scrolled_Event e(xoffset,yoffset);
			window_data.event_callback(e);
		}
		);


		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window,double x,double y)
		{
			
			Window_Data& window_data = *(Window_Data*)glfwGetWindowUserPointer(window);

			Mouse_Moved_Event e(x,y);
			window_data.event_callback(e);

		}
		);

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


	std::unique_ptr<Window> Window::create(const Window_Property& p)
	{
		return std::make_unique<Windows_Window>(p);
	}

}