


#pragma once

#include "application.h"
#include "log.h"
#include "profiler.h"
#include <memory>


int main(int argc,char** argv)
{

	BEGIN_PROFILING();

	clumsy_engine::run_test(argc,argv);

	clumsy_engine::Log::init();

	CE_CORE_TRACE("core trace");
	CE_CORE_INFO("core info");
	CE_CORE_WARN("core warn");

	CE_WARN("core warn");
	int a = 1;
	CE_WARN("core warn a={0}",a);

	std::unique_ptr<clumsy_engine::Application> app = clumsy_engine::create_application();

	END_PROFILING();

	app->run();

}
