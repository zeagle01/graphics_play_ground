
#include <cmath>

#include "Drawing_Buffer.h"


int main()
{
	Drawing_Buffer screen;

	int width = 800;
	int height = 600;
	screen.init(width, height);

	float t = 0.f;
	screen.main_loop([&screen, width, height, &t]()
		{

			float dt = 0.01;
			float pi = 3.1415926f;

			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					float r = (0.3f * (std::sin(t * pi + 1.f * i / width * pi * 6) + 1.f) + 0.4f);
					float b = 1.f * j / height * (0.3f * (std::cos(t * pi) + 1.f) + 0.4f);
					screen.set_color(i, j, r, 0.f, b);
				}
			}
			t += dt;

		});

	return 0;
}


