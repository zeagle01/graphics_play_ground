

#pragma once

#include <string>

namespace clumsy_engine
{

	class File_Dialogs
	{
	public:
		static std::string open_file(const char* filter);
		static std::string save_file(const char* filter);
	};

}
