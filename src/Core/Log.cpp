#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_console;

void Log::Init()
{
    spdlog::set_pattern("%n [%T]:%^%v%$");
    s_console = spdlog::stdout_color_mt("CONSOLE");

    s_console->set_level(spdlog::level::trace);
}