// Copyright (c) Eric Jeker 2025.

#include "Core/Utils/Logger.h"

#include <iostream>
#include <ostream>

namespace
{

std::string SeverityToText(const Logger::LogLevel level)
{
    switch (level)
    {
        case Logger::LogLevel::Trace:
            return "TRACE";
        case Logger::LogLevel::Debug:
            return "DEBUG";
        case Logger::LogLevel::Info:
            return "INFO";
        case Logger::LogLevel::Warning:
            return "WARN";
        case Logger::LogLevel::Error:
            return "ERROR";
        case Logger::LogLevel::Fatal:
            return "FATAL";
    }

    return "UNKNOWN";
}

void Log(const Logger::LogLevel level, const std::string& message)
{
    // Use std::cerr when the log level is below or equal Error
    auto& oStream = (level <= Logger::LogLevel::Error) ? std::cerr : std::cout;
    oStream << " [" << SeverityToText(level) << "] " << message << std::endl;
}

} // namespace

void Logger::Fatal(const std::string& message)
{
    Log(LogLevel::Fatal, message);
}

void Logger::Error(const std::string& message)
{
    Log(LogLevel::Error, message);
}

void Logger::Warn(const std::string& message)
{
    Log(LogLevel::Warning, message);
}

void Logger::Info(const std::string& message)
{
    Log(LogLevel::Info, message);
}

void Logger::Debug(const std::string& message)
{
    Log(LogLevel::Debug, message);
}

void Logger::Trace(const std::string& message)
{
    Log(LogLevel::Trace, message);
}
