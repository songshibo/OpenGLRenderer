#pragma once

#include "spdlog/spdlog.h"

class Log
{
private:
    static std::shared_ptr<spdlog::logger> s_console;

public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger> &Logger() { return s_console; }
};

// Console log marcos
#define DEBUG_TRACE(...) ::Log::Logger()->trace(__VA_ARGS__)
#define DEBUG_INFO(...) ::Log::Logger()->info(__VA_ARGS__)
#define DEBUG_WARN(...) ::Log::Logger()->warn(__VA_ARGS__)
#define DEBUG_ERROR(...) ::Log::Logger()->error(__VA_ARGS__)
#define DEBUG_FATAL(...) ::Log::Logger()->critical(__VA_ARGS__)