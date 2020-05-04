
#include <windows.h>

#include <iostream>
#include <algorithm>
#include <thread>



void line(int x0, int y0, int x1, int y1, COLORREF color,HDC dc)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x = x0; x <= x1; x++)
	{
		float t = (float)(y1 - y0) /(float) (x1 - x0);
		int y = y0 + t * (x - x0);
		if (steep)
		{
			SetPixel(dc, y, x, color);
		}
		else 
		{
			SetPixel(dc, x, y, color);
		}
	}
}

int main()
{
	HWND my_console = GetConsoleWindow();
	HDC my_dc = GetDC(my_console);

	const int width = 800;
	const int height = 600;
	RECT r;
	GetWindowRect(my_console, &r);
	MoveWindow(my_console, r.left, r.top, width, height, TRUE);


	float t = 0.f;
	while (true)
	{
		system("cls");
		float mag = 0.3 * width;
		float offset = mag*std::sin(t);

		line(200 + offset, 300, 500, 200, RGB(255, 255, 255), my_dc);
		line(300, 500 + offset, 200, 200, RGB(0, 255, 100), my_dc);
		t += .01;

//		using namespace std::literals;
//		std::this_thread::sleep_for(2000ms);
	}

	ReleaseDC(my_console, my_dc);

	return 0;
}


