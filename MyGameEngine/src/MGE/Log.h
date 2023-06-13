#pragma once 

#include <memory.h>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace MGE {

	class MGE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// core log macros
#define MGE_CORE_ERROR(...)      ::MGE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MGE_CORE_WARN(...)       ::MGE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MGE_CORE_TRACE(...)      ::MGE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MGE_CORE_INFO(...)       ::MGE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MGE_CORE_FATAL(...)      ::MGE::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client log macros
#define MGE_CLIENT_ERROR(...)    ::MGE::Log::GetClientLogger()->error(__VA_ARGS__)
#define MGE_CLIENT_WARN(...)     ::MGE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MGE_CLIENT_TRACE(...)    ::MGE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MGE_CLIENT_INFO(...)     ::MGE::Log::GetClientLogger()->info(__VA_ARGS__)
#define MGE_CLIENT_FATAL(...)    ::MGE::Log::GetClientLogger()->fatal(__VA_ARGS__)