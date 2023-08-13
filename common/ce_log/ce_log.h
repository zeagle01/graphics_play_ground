#pragma once

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace  ce_log
{
	class Log
	{
	public:
		static void init();
		static std::shared_ptr<spdlog::logger> get_core_logger();

	private:
		static inline std::shared_ptr<spdlog::logger> s_core_logger = nullptr;
	};
}


#define CE_LOG_ERROR(...)	ce_log::Log::get_core_logger()->error(__VA_ARGS__)
#define CE_LOG_WARN(...)	ce_log::Log::get_core_logger()->warn(__VA_ARGS__)
#define CE_LOG_INFO(...)	ce_log::Log::get_core_logger()->info(__VA_ARGS__)
#define CE_LOG_TRACE(...)	ce_log::Log::get_core_logger()->trace(__VA_ARGS__)
#define CE_LOG_FATAL(...)	ce_log::Log::get_core_logger()->fatal(__VA_ARGS__)

