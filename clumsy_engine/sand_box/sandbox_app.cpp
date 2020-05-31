
#include <iostream>

#include "clumsy_engine/clumsy_engine.h"



class SanBox_App:public clumsy_engine::Application

{

};



clumsy_engine::Application* clumsy_engine::create_application()
{
	clumsy_engine::Log::get_core_logger()->trace("create app");

	return new clumsy_engine::Application();
}


