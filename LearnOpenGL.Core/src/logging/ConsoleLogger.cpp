#include "ConsoleLogger.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <color-console/color.hpp>

using namespace std::string_literals;

namespace logging
{
    ConsoleLogger::ConsoleLogger() : ILogger{}
    {
    }

    ConsoleLogger::ConsoleLogger(const LogLevel& logLevel) : ILogger{}
    {
        ConsoleLogger::SetLogLevel(logLevel);
    }

    void ConsoleLogger::SetLogLevel(const LogLevel& logLevel)
    {
        this->logLevel = logLevel;
    }

    const LogLevel& ConsoleLogger::GetLogLevel() const
    {
        return logLevel;
    }

    void ConsoleLogger::WriteLine(const char* msg) const
    {
        time_t now = std::time(nullptr);
        tm localTime{};
        localtime_s(&localTime, &now);
        std::ostringstream time_ss{};
        time_ss << std::put_time(&localTime, "%H:%M:%S");
        const std::string& timeString{time_ss.str()};
        switch (logLevel)
        {
        case LogLevel::Debug:
            std::cout << dye::light_blue(timeString + "-[DEBUG]"s) << dye::green("=> ");
            break;
        case LogLevel::Warning:
            std::cout << dye::yellow(timeString + "-[DEBUG]"s) << dye::green("=> ");
            break;
        case LogLevel::Error:
            std::cout << dye::light_red(timeString + "-[DEBUG]"s) << dye::green("=> ");
            break;
        default:
            throw std::exception("Not supported");
        }

        std::cout << msg << '\n';
    }
}
