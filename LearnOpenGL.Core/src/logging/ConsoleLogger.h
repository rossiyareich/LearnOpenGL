﻿#pragma once
#include <memory>
#include <string>

#include "ILogger.h"

namespace logging
{
    class ConsoleLogger : public ILogger
    {
    public:
        static ConsoleLogger& Get();
        ConsoleLogger(const ConsoleLogger&) = delete;
        ConsoleLogger(ConsoleLogger&&) = delete;
        ConsoleLogger& operator=(const ConsoleLogger&) = delete;
        ConsoleLogger& operator=(ConsoleLogger&&) = delete;

        void WriteLine(const char* msg) const override;
        void WriteLine(const std::string& msg) const override;
        void SetLogLevel(const LogLevel& logLevel) override;
        void WriteLine(const char* msg, LogLevel logLevel) override;
        void WriteLine(const std::string& msg, LogLevel logLevel) override;
        const LogLevel& GetLogLevel() const override;
    private:
        ConsoleLogger();
        void WriteNoMsg() const;
    };
}
