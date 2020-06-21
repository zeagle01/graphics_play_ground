

#include "log.h"
#include <string>

#include "profiler.h"


namespace clumsy_engine
{
	std::shared_ptr<spdlog::logger> Log::s_core_logger ;
	std::shared_ptr<spdlog::logger> Log::s_client_logger ;
	void Log::init()
	{


		RECORD_FUNCTION_DURATION();

		std::string format = "%^%T:%f %n: %v%$";
		//std::string format = "%+";
		s_core_logger = spdlog::stdout_color_mt("core");
		s_core_logger->set_pattern(format);
		s_core_logger->set_level(spdlog::level::trace);

		s_client_logger = spdlog::stdout_color_mt("client");
		s_client_logger->set_pattern(format);
		s_client_logger->set_level(spdlog::level::trace);

	}

	std::shared_ptr<spdlog::logger>	Log::get_core_logger()
	{
		if (!s_core_logger)
		{
			init();
		}

		return s_core_logger;

	}

	std::shared_ptr<spdlog::logger> Log::get_client_logger()
	{
		if (!s_client_logger)
		{
			init();
		}
		return s_client_logger;

	}

}
