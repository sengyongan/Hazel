#pragma once
//日志库更改接口
#include<memory>//使用头指针

#include "core.h"
#include"spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
namespace Hazel {
	class HAZEL_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();
		//内联函数，std::shared_ptr<spdlog::logger>&返回类型，spdlog::logger的指针
		//客户端日志和核心日志
		inline  static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline  static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }
	};

}
//预定义宏
//core
#define HZ_CORE_TRACE(...)	::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)	::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)	::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)	::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...)	::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__)
//client
#define HZ_CLIENT_TRACE(...)	::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_CLIENT_INFO(...)		::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_CLIENT_WARN(...)		::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_CLIENT_ERROR(...)	::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_CLIENT_FATAL(...)	::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__)
					