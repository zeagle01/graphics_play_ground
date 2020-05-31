


#pragma once

#include "application.h"
#include "log.h"

extern clumsy_engine::Application* clumsy_engine::create_application();

int main(int argc,char** argv)
{
	clumsy_engine::Log::init();

	CE_CORE_TRACE("core trace");
	CE_CORE_INFO("core info");
	CE_CORE_WARN("core warn");

	CE_WARN("core warn");
	int a = 1;
	CE_WARN("core warn a={0}",a);

	auto app = clumsy_engine::create_application();
	app->run();
	delete app;
}
