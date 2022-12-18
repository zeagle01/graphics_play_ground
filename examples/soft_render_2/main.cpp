
#include "Spinning_Cube_App.h"
#include "serialization.h"
#include <iostream>

#include "Spinning_Cube.h"


int main()
{
	soft_render::Spinning_Cube_App app;

	app.run();

	soft_render::serilizer::write_type_map(app, std::cout);

	return 0;
}


