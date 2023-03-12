
#include "Spinning_Cube_App.h"
#include "serialization.h"
#include <iostream>
#include <fstream>

#include "Spinning_Cube.h"
#include "Camara.h"


int main()
{

	soft_render::Spinning_Cube_App app; 
	app.init();

	std::string project_file_name = "app.txt";
	std::ifstream fin(project_file_name);

	if (!soft_render::serilizer::read<typename soft_render::Spinning_Cube_App>(app, fin))
	{
		printf(" read cache file failed !\n");

	}

	app.run();

	std::ofstream fout(project_file_name);
	soft_render::serilizer::write(app, fout);
	soft_render::serilizer::write(app, std::cout);

	return 0;
}


