
#include <windows.h>
int main()
{
	HWND my_console = GetConsoleWindow();
	HDC my_dc = GetDC(my_console);
	for (int i = 0; i < 100; i++)
	{

		for (int j = 0; j < 1000; j++)
		{
			SetPixel(my_dc, i, j, RGB(255, 0, 0));
		}
	}

	ReleaseDC(my_console, my_dc);
	return 0;
}


