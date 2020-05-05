
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

struct vec2i
{
	int x;
	int y;
	vec2i operator+(const vec2i& other)
	{
		vec2i ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}

	vec2i operator-(const vec2i& other)
	{
		return { x + other.x,y + other.y };
	}

	vec2i operator*(float scalar)
	{
		return { int(scalar * x),int(scalar * y) };
	}
};

void fill_triangle(vec2i v0, vec2i v1, vec2i v2, COLORREF color, HDC dc)
{
	if (v0.y > v1.y) std::swap(v0, v1);
	if (v0.y > v2.y) std::swap(v0, v2);
	if (v1.y > v2.y) std::swap(v1, v2);
	//int total_height
	for (int y = v0.y; y <= v1.y; y++)
	{
		float t1 = float(y - v0.y) / float(v1.y - v0.y);
		float t2 = float(y - v0.y) / float(v2.y - v0.y);
		vec2i A = v0 + (v1 - v0) * t1;
		vec2i B = v1 + (v2 - v0) * t2;
		line(A.x, A.y, B.x, B.y, color, dc);
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
	//mesh_loader.load_from_obj("cases/african_head.obj");
    //mesh_loader.load_from_obj("cases/flag.obj");
    mesh_loader.load_from_obj("cases/1_triangle.obj");
	auto positions = mesh_loader.get_positions();
	auto indices = mesh_loader.get_indices();

	float t = 0.f;
//	while (true)
//	{
		system("cls");

		for (size_t ti = 0; ti < indices.size()/3; ti++)
		{
			std::vector<vec2i> tv;
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
				tv.push_back({ x0,y0 });
			}
			fill_triangle(tv[0], tv[1], tv[2], RGB(100, 255, 0), my_dc);
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


