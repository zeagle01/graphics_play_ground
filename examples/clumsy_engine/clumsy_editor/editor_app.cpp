

#include "editor_layer.h"

#include "clumsy_engine/clumsy_engine.h"
#include "clumsy_engine/entry_point.h"//only included once

#include "log.h"

namespace clumsy_engine
{
	class clumsy_editor :public Application
	{
	public:
		clumsy_editor():
			Application("clumsy_editor")
		{
			push_layer(std::make_shared<editor_layer>());
		}

	};

	std::unique_ptr<clumsy_engine::Application> create_application()
	{

		RECORD_FUNCTION_DURATION();

		Log::get_core_logger()->trace("create app");

		std::unique_ptr<Application> app = std::make_unique<clumsy_editor>();

		return app;
	}

}

