
#include <windows.h>

#include <iostream>


void line(int x0, int y0, int x1, int y1, COLORREF color,HDC dc)
{
	for (float t = 0.f; t < 1.f; t += 0.01f)
	{
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		SetPixel(dc, x, y, color);
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

		line(300 + t, 300 , 200, 200, RGB(255, 100, 100), my_dc);
		t += 1.0;
	}

	ReleaseDC(my_console, my_dc);
	int a;
	std::cin >> a;

	return 0;
}


