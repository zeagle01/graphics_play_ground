


#include <iostream>

#include "gmock/gmock.h"

#include "soft_render.h"





using namespace soft_render;




//void fill_triangle(vec2i v0, vec2i v1, vec2i v2, COLORREF color, HDC dc)
//{
//	if (v0.y > v1.y) std::swap(v0, v1);
//	if (v0.y > v2.y) std::swap(v0, v2);
//	if (v1.y > v2.y) std::swap(v1, v2);
//	//int total_height
//	for (int y = v0.y; y < v1.y; y++)
//	{
//		float t1 = float(y - v0.y) / float(v1.y - v0.y);
//		float t2 = float(y - v0.y) / float(v2.y - v0.y);
//		int xa = v0.x + (v1.x - v0.x) * t1;
//		int xb = v0.x + (v2.x - v0.x) * t2;
//
//		line(xa, y, xb, y, color, dc);
//	}
//
//	for (int y = v1.y; y < v2.y; y++)
//	{
//		float t1 = float(y - v1.y) / float(v2.y - v1.y);
//		float t2 = float(y - v0.y) / float(v2.y - v0.y);
//		int xa = v1.x + (v2.x - v1.x) * t1;
//		int xb = v0.x + (v2.x - v0.x) * t2;
//
//		line(xa, y, xb, y, color, dc);
//	}
//
//}




int main(int argc, char** argv)
{

	testing::InitGoogleMock(&argc, argv);
	RUN_ALL_TESTS();

	getchar();


	Plain_Renderer rander;
	rander.render();

	int c;
	std::cin >> c;

	return 0;
}


