

#include "ce_log.h"

#include <string>


namespace ce_log
{
	void Log::init()
	{

		std::string format = "%^%T:%f %n: %v%$";
		//std::string format = "%+";
		s_core_logger = spdlog::stdout_color_mt("core");
		s_core_logger->set_pattern(format);
		s_core_logger->set_level(spdlog::level::trace);

	}

	std::shared_ptr<spdlog::logger>	Log::get_core_logger()
	{
		if (!s_core_logger)
		{
			init();
		}

		return s_core_logger;

	}

}
