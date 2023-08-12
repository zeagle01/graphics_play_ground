


#pragma once
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

#include "clumsy_engine_export.h"


namespace  clumsy_engine
{
	class CLUMSY_ENGINE_EXPORT Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>get_core_logger();
		inline static std::shared_ptr<spdlog::logger>get_client_logger();

	private:
		static std::shared_ptr<spdlog::logger> s_core_logger;
		static std::shared_ptr<spdlog::logger> s_client_logger;
	};
}


#define CE_CORE_ERROR(...)	clumsy_engine::Log::get_core_logger()->error(__VA_ARGS__)
#define CE_CORE_WARN(...)	clumsy_engine::Log::get_core_logger()->warn(__VA_ARGS__)
#define CE_CORE_INFO(...)	clumsy_engine::Log::get_core_logger()->info(__VA_ARGS__)
#define CE_CORE_TRACE(...)	clumsy_engine::Log::get_core_logger()->trace(__VA_ARGS__)
#define CE_CORE_FATAL(...)	clumsy_engine::Log::get_core_logger()->fatal(__VA_ARGS__)

#define CE_ERROR(...)	clumsy_engine::Log::get_client_logger()->error(__VA_ARGS__)
#define CE_WARN(...)	clumsy_engine::Log::get_client_logger()->warn(__VA_ARGS__)
#define CE_INFO(...)	clumsy_engine::Log::get_client_logger()->info(__VA_ARGS__)
#define CE_TRACE(...)	clumsy_engine::Log::get_client_logger()->trace(__VA_ARGS__)
#define CE_FATAL(...)	clumsy_engine::Log::get_client_logger()->fatal(__VA_ARGS__)

