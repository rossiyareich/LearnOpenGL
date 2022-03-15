#include "ConsoleLogger.h"

#include <cassert>
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

    ConsoleLogger& ConsoleLogger::Get()
    {
        static ConsoleLogger logger{};
        return logger;
    }


    void ConsoleLogger::SetLogLevel(const LogLevel& logLevel)
    {
        this->logLevel = logLevel;
    }

    const LogLevel& ConsoleLogger::GetLogLevel() const
    {
        return logLevel;
    }

    void ConsoleLogger::WriteNoMsg() const
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
            std::cout << dye::yellow(timeString + "-[WARNING]"s) << dye::green("=> ");
            break;
        case LogLevel::Error:
            std::cout << dye::light_red(timeString + "-[ERROR]"s) << dye::green("=> ");
            break;
        default:
            throw std::exception("Not supported");
        }
    }

    void ConsoleLogger::WriteLine(const char* msg) const
    {
        WriteNoMsg();
        std::cout << msg << '\n';
    }

    void ConsoleLogger::WriteLine(const std::string& msg) const
    {
        WriteNoMsg();
        std::cout << msg << '\n';
    }
}
