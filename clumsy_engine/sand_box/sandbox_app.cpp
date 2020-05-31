
#include <iostream>

#include "clumsy_engine/clumsy_engine.h"



class SanBox_App:public clumsy_engine::Application

{

};



clumsy_engine::Application* clumsy_engine::create_application()
{
	return new clumsy_engine::Application();
}


