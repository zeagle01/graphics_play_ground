


#pragma once

#include "application.h"

extern clumsy_engine::Application* clumsy_engine::create_application();

int main(int argc,char** argv)
{
	auto app = clumsy_engine::create_application();
	app->run();
	delete app;
}
