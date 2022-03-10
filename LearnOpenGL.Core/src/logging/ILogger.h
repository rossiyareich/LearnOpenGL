#pragma once
#include "LogLevel.h"

namespace logging
{
    class ILogger
    {
    public:
        ILogger() : logLevel(LogLevel::Debug)
        {
        }

        virtual ~ILogger() = default;
        virtual void WriteLine(const char* msg) const = 0;
        virtual void SetLogLevel(const LogLevel& logLevel) = 0;
        virtual const LogLevel& GetLogLevel() const = 0;
    protected:
        LogLevel logLevel;
    };
}
