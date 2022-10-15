
#include "Drawing_Buffer.h"



int main()
{
	Drawing_Buffer screen;

	int width = 800;
	int height = 600;
	screen.init(width, height);

	screen.main_loop([width, height](uint32_t* data)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{

					//int b = (1.f * i / width) * 255;
					int b = 0;
					int g = (1.f * j / height) * 255;
					//int g = 0;
					int r = 0;
					data[i * height + j] = (255 << 24) + (int(r) << 16) + (int(g) << 8) + int(b);

				}
			}

		});

	return 0;
}


