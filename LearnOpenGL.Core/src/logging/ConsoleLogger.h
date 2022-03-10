#pragma once
#include "ILogger.h"

namespace logging
{
    class ConsoleLogger : public ILogger
    {
    public:
        ConsoleLogger();
        ConsoleLogger(const LogLevel& logLevel);
        void WriteLine(const char* msg) const override;
        void SetLogLevel(const LogLevel& logLevel) override;
        const LogLevel& GetLogLevel() const override;
    private:
    };
}
