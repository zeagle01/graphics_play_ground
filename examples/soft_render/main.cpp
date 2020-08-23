


#include <iostream>

#include "gmock/gmock.h"

#include "soft_render.h"





using namespace soft_render;








int main(int argc, char** argv)
{

	testing::InitGoogleMock(&argc, argv);
	RUN_ALL_TESTS();

	getchar();


	//Plain_Renderer rander;
	PipeLine_Renderer rander;
	rander.render();

	int c;
	std::cin >> c;

	return 0;
}


