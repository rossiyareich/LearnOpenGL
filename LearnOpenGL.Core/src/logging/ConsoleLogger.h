#pragma once
#include <memory>
#include <string>

#include "ILogger.h"

namespace logging
{
    class ConsoleLogger : public ILogger
    {
    public:
        static ConsoleLogger& Get();
        ConsoleLogger(ConsoleLogger const&) = delete;
        ConsoleLogger(ConsoleLogger&&) = delete;
        ConsoleLogger& operator=(ConsoleLogger const&) = delete;
        ConsoleLogger& operator=(ConsoleLogger&&) = delete;

        void WriteLine(const char* msg) const override;
        void WriteLine(const std::string& msg) const override;
        void SetLogLevel(const LogLevel& logLevel) override;
        const LogLevel& GetLogLevel() const override;
    private:
        ConsoleLogger();
        void WriteNoMsg() const;
    };
}
