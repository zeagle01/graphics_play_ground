

#include "input.h"
#include <memory>
#include "windows_input.h"

namespace clumsy_engine
{



	Input& Input::get_singleton()
	{
		static std::unique_ptr<Input> singleton = std::make_unique<Windows_Input>();
		return *singleton;
	}

}