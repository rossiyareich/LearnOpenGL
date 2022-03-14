#pragma once
#include <string>

#include "ILogger.h"

namespace logging
{
    class ConsoleLogger : public ILogger
    {
    public:
        static ConsoleLogger* Logger;
        ConsoleLogger();
        ConsoleLogger(const LogLevel& logLevel);
        void WriteLine(const char* msg) const override;
        void WriteLine(const std::string& msg) const override;
        void SetLogLevel(const LogLevel& logLevel) override;
        const LogLevel& GetLogLevel() const override;
        ~ConsoleLogger() override;
    private:
        void WriteNoMsg() const;
    };
}
