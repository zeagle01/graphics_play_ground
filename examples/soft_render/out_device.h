
#pragma once

#include "frame_buffer.h"
#include <windows.h>

namespace soft_render
{
	class Out_Device
	{
	public:
		virtual void flush(Frame_Buffer& buffer) 
		{

			HWND my_console = GetConsoleWindow();
			HDC my_dc = GetDC(my_console);

			RECT r;
			GetWindowRect(my_console, &r);
			MoveWindow(my_console, r.left, r.top, buffer.get_width(), buffer.get_height(), TRUE);


			for (int x = 0; x < buffer.get_width(); x++)
			{
				for (int y = 0; y < buffer.get_height(); y++)
				{

					vec4i color = buffer.get_color_i(x, y);
					SetPixel(my_dc, x, y, RGB(color[0] , color[1] , color[2] ));
				}
			}

			ReleaseDC(my_console, my_dc);
		};
	};

}
