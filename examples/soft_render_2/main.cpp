
#include "Drawing_Buffer.h"

int main()
{
	Drawing_Buffer screen;

	int width = 800;
	int height = 600;
	screen.init(width, height);

	screen.main_loop([width, height](uint32_t* data)
		{
			for (int i = 0; i < width * height; i++)
			{
				data[i]++;
			}
		});

	return 0;
}


