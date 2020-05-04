
#include <windows.h>

#include <iostream>
#include <algorithm>
#include <thread>
#include <Mesh_Loader.h>



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

	Mesh_Loader mesh_loader;
    mesh_loader.load_from_obj("cases/african_head.obj");
    //mesh_loader.load_from_obj("cases/flag.obj");
	auto positions = mesh_loader.get_positions();
	auto indices = mesh_loader.get_indices();

	float t = 0.f;
//	while (true)
//	{
		system("cls");

		for (size_t ti = 0; ti < indices.size()/3; ti++)
		{
			for (int vi = 0; vi < 3; vi++)
			{
				int vi_next = (vi + 1) % 3;
				int v = indices[ti * 3 + vi];
				int v_next = indices[ti * 3 + vi_next];

				int x0 = (positions[v * 3 + 0] + 1.0) * width * 0.5;
				int y0 = (1.f - positions[v * 3 + 1]) * height * 0.5;
				int x1 = (positions[v_next * 3 + 0] + 1.0) * width * 0.5;
				int y1 = (1.f - positions[v_next * 3 + 1]) * height * 0.5;

				line(x0, y0, x1, y1, RGB(255, 100, 0), my_dc);
			}

		}

		t += .01;

//		using namespace std::literals;
//		std::this_thread::sleep_for(2000ms);
//	}

	ReleaseDC(my_console, my_dc);

	int c;
	std::cin >> c;

	return 0;
}


