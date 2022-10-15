
#include "Drawing_Buffer.h"

int main()
{
	Drawing_Buffer screen;

	screen.init(800, 600);

	screen.main_loop([](void* data)
		{
			uint32_t* a = reinterpret_cast<uint32_t*>(data);
			for (int i = 0; i < 800 * 600; i++)
			{
				a[i]++;
			}
		});

	return 0;
}


